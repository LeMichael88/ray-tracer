#include "RayTracer.hpp"

#include <iomanip>
#include <sstream>
#include <thread>
#include <vector>

#include <glm/ext.hpp>

//---------------------------------------------------------------------------------------
RayTracer::RayTracer(
	SceneNode* root,
	const glm::mat4& screenToWorld,
	const glm::vec3& eye,
	const glm::vec3& ambient,
	const std::list<Light*>& lights,
	Image* image)
	: m_root(root),
		m_screenToWorld(screenToWorld),
		m_eye(eye),
		m_ambient(ambient),
		m_lights(lights),
		m_image(image)
{}

//---------------------------------------------------------------------------------------
// Traverses the scene graph and adds all animations
void RayTracer::preprocessAnimation(SceneNode* node, const float t)
{
	node->animateNode(t);

	for (SceneNode* child : node->children)
	{
		preprocessAnimation(child, t);
	}
}

//---------------------------------------------------------------------------------------
// Traverses the scene graph and resets all animations
void RayTracer::resetAnimation(SceneNode* node)
{
	node->resetAnimation();

	for (SceneNode* child : node->children)
	{
		resetAnimation(child);
	}
}

//---------------------------------------------------------------------------------------
// Traverses the hierarchial scene graph starting at node for an intersection with ray
// Modifies intersection to the closest intersection if there is one
// trans and invTrans are used to pass hierarchial transformations down the tree
void RayTracer::traverseSceneGraph(
	const SceneNode* node,
	Ray ray,
	Intersection& intersection,
	glm::mat4 trans,
	glm::mat4 invTrans
) const {
	// Add transformations of the current node as we go "down" the tree
	trans = trans * node->get_transform();
	invTrans = node->get_inverse() * invTrans;

	// Transform the ray into model coordinates of this node
	ray.transform(invTrans);

	// Find intersection with this node and check if closer than an existing intersection
	Intersection temp = node->intersect(ray);
	temp.transformIntersection(trans); // Intersection converted to world coords
	temp.m_point += 0.25f * temp.m_normal; // Add fudge factor
	if (temp.m_foundIntersection && intersection.isPointCloser(temp.m_point, ray))
	{
		intersection = temp;
	}

	// Recursively check for more intersections with children nodes
	for (const SceneNode* child : node->children)
	{
		traverseSceneGraph(child, ray, intersection, trans, invTrans);
	}
}

//---------------------------------------------------------------------------------------
// Trace the path of a ray and get a colour for it
Color RayTracer::raytrace(Ray& ray, const int currDepth) const {
	// Default background gradient
	float blend = 0.5f * (glm::normalize(ray.direction()).y + 1.0f);
	glm::vec3 color = (1.0f - blend) * glm::vec3(1.0f, 0.235f, 0.976f) 
										+ blend * glm::vec3(0.224f, 0.996f, 1.0f);

	// Get closest intersection to an object
	Intersection intersection;
	traverseSceneGraph(m_root, ray, intersection, glm::mat4(), glm::mat4());

	// If there is no intersection with an object, just return background color
	// Note: we do not care about primary intersections with light source
	if (!intersection.m_foundIntersection)
	{
		return color;
	}

	// Compute ambient component once we have an intersection
	color = intersection.m_material->kd(intersection.m_uv) * m_ambient;

	// Send rays from intersection point to each light source
	std::vector<Light*> unblockedLights;
	for (Light* light : m_lights)
	{
		glm::vec3 shadowDir = light->m_position - intersection.m_point; // Not normalized
		Ray shadow = Ray(intersection.m_point, shadowDir, ray.time());

		// Loop through every geometry
		Intersection _; // We don't care about the point itself, just if it intersects
		_.m_point = light->m_position; // Intersections shouldn't be past light pos
		_.m_isLight = true;
		traverseSceneGraph(m_root, shadow, _, glm::mat4(), glm::mat4());

		// Only add to unblocked lights if there are no intersections
		if (!_.m_foundIntersection)
		{
			unblockedLights.push_back(light);
		}
	}

	// Compute v = vector from point to eye and normalize
	glm::vec3 v = glm::normalize(m_eye - intersection.m_point);

	// Sum the light from diffuse and specular components for each unblocked light
	glm::vec3 diffuseComponent(0.0f);
	glm::vec3 specularComponent(0.0f);
	for (const Light* light : unblockedLights)
	{
		// Compute l = vector from point to light and normalize (and save distance)
		glm::vec3 l = light->m_position - intersection.m_point;
		float r = glm::length(l);
		l /= r;

		// Compute h = half vector and normalize
		glm::vec3 h = glm::normalize(l + v);

		// Compute attenuation factor for this light source
		float attenuation = light->m_falloff[0] + 
												light->m_falloff[1] * r + 
												light->m_falloff[2] * pow(r, 2);

		// Finally, compute the contribution of this light source to color
		glm::vec3 common = light->m_colour / attenuation;
		diffuseComponent += (std::max(0.0f, glm::dot(l, intersection.m_normal))
												 * common);
		specularComponent += (pow(glm::dot(h, intersection.m_normal), 
															intersection.m_material->shininess()) 
													* common);
	}

	// Recursively send rays to get color of reflection
	// Stop recursion after some number of reflections or if material does not reflect
	Color reflectedColor(0.0f);
	if (currDepth < MAX_DEPTH && 
			intersection.m_material->kr(intersection.m_uv) != glm::vec3(0.0f)) {
		glm::vec3 normalized_i = glm::normalize(ray.direction());
		glm::vec3 reflectedDir = normalized_i - 
														 2 * intersection.m_normal * 
														 glm::dot(normalized_i, intersection.m_normal);
		Ray reflected(intersection.m_point, reflectedDir, ray.time());
		reflectedColor = raytrace(reflected, currDepth + 1);
	}

	// Post multiply diffuse and specular components by k_d and k_s
	color += intersection.m_material->kd(intersection.m_uv) * diffuseComponent + 
					 intersection.m_material->ks(intersection.m_uv) * specularComponent +
					 intersection.m_material->kr(intersection.m_uv) * reflectedColor;
	return color;
}

//---------------------------------------------------------------------------------------
// Render a part of an image using multithreaded raytracing
void RayTracer::render(
	const int frameNum,
	const int threadNum,
	const int startWidth,
	const int endWidth
) const {
	// Iterate through each pixel of the image
	const int percentage_interval = 10.0f;
	int interval = (endWidth - startWidth) / percentage_interval;
	for (int i = startWidth; i < endWidth; i++)
	{
		for (int j = 0; j < (*m_image).height(); j++)
		{
			Color color(0.0f);
			for (int k = 0; k < SAMPLE_SIZE; k++)
			{
				// Jitter the ray a little between [-0.5, 0.5]
				glm::vec4 p(i, j, 0, 1); // Set z = 0 at first
				p.x += (static_cast<float>(std::rand()) / RAND_MAX) - 0.5f;
				p.y += (static_cast<float>(std::rand()) / RAND_MAX) - 0.5f;

				// Convert from screen coordinates to world coordinates
				glm::vec4 p_world = m_screenToWorld * p;

				// Create a ray from camera to pixel in world coordinates
				glm::vec4 direction = p_world - glm::vec4(m_eye, 0);
				Ray ray = Ray(m_eye, glm::vec3(direction), frameNum);
				ray.m_id = i * (*m_image).height() + j;

				// Check for ray intersections with objects
				color += raytrace(ray, 0);
			}

			// Average the samples
			color /= static_cast<float>(SAMPLE_SIZE);

			// Set colour based on intersection (RGB)
			(*m_image)(i, j, 0) = (double)color.r;
			(*m_image)(i, j, 1) = (double)color.g;
			(*m_image)(i, j, 2) = (double)color.b;
		}

#ifdef DEBUG_LOGS
		if ((i - startWidth) % interval == 0)
		{
			std::cout << " - Rendering thread " << threadNum << ": "
								<< ((i - startWidth) / interval) * percentage_interval << "%" 
								<< std::endl;
		}
#endif
	}
}

//---------------------------------------------------------------------------------------
// Render an image using raytracing
void A5_Render(
	// What to render  
	SceneNode* root,

	// Image to write to, set to a given width and height  
	Image& image,
	const char* fileName,
	const int startFrame,
	const int numFrames,

	// Viewing parameters  
	const glm::vec3& eye,
	const glm::vec3& view,
	const glm::vec3& up,
	double fovy,

	// Lighting parameters  
	const glm::vec3& ambient,
	const std::list<Light*>& lights,
	const std::list<ParticleNode*>& particleSpawners
) {
	// Set up animations for lights here (lua is hard)
	int counter = 0;
	for (Light* light : lights)
	{
		if (counter == 0)
		{
			light->m_animation = Animation(124, 194, 't', "down");
		}
		else
		{
			light->m_animation = Animation(124, 194, 't', "up");
		}
		counter++;
	}
	
	float cameraStartTime = 218;
	float cameraEndTime = 266;
	Animation cameraMovement = Animation(cameraStartTime, cameraEndTime, 't', "camera");
	Animation viewMovement = Animation(cameraStartTime, cameraEndTime, 't', "view");

  /*
	 * Ray Tracing Main Function Code
	 */

	// Render each frame
	for (int frame = startFrame; frame < (startFrame + numFrames); frame++)
	{
		// Animations for the camera and view (lua is hard)
		glm::vec3 updatedEye = eye;
		glm::vec3 updatedView = view;
		if (frame > cameraEndTime) // Full translation if animation is completed
		{
			updatedEye = cameraMovement.m_animation(cameraEndTime - cameraStartTime) + eye;
			updatedView = viewMovement.m_animation(cameraEndTime - cameraStartTime) + view;
		}
		else if (frame >= cameraStartTime) // We are in the middle of this animation
		{
			updatedEye = cameraMovement.m_animation(frame - cameraStartTime) + eye;
			updatedView = viewMovement.m_animation(frame - cameraStartTime) + view;
		}

		/*
		* Transformation matricies to create viewing rays
		*/
		const int nx = static_cast<int>(image.width());
		const int ny = static_cast<int>(image.height());
		const float d = 1.0f;
		glm::mat4 T1 = glm::translate(glm::mat4(), 
																	glm::vec3(
																		-1 * nx / 2, 
																		-1 * ny / 2, 
																		d
																	));
		
		float height = 2 * d * tan(glm::radians(fovy) / 2);
		float width = (nx / ny) * height;
		glm::mat4 S2 = glm::scale(glm::mat4(), 
															glm::vec3(
																-1.0f * height / ny,
																-1.0f * width / nx, // Flip on y as well
																1
															));
		
		glm::vec3 r3_w = glm::normalize(updatedView - updatedEye);
		glm::vec3 r3_u = glm::normalize(glm::cross(up, r3_w));
		glm::vec3 r3_v = glm::cross(r3_w, r3_u);
		glm::mat4 R3 = glm::mat4(
			glm::vec4(r3_u, 0),
			glm::vec4(r3_v, 0),
			glm::vec4(r3_w, 0),
			glm::vec4(0, 0, 0, 1)
		);

		glm::mat4 T4 = glm::mat4(
			glm::vec4(1, 0, 0, 0),
			glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0),
			glm::vec4(updatedEye, 1)
		);

		glm::mat4 M = T4 * R3 * S2 * T1; // This is the final matrix for screen -> world

		// Create RayTracer object
		RayTracer raytracer(root, M, updatedEye, ambient, lights, &image);

		// At the start of each frame, preprocess each particle system
		for (ParticleNode* particles : particleSpawners)
		{
			particles->preprocessParticles(frame);
		}

		// Preprocess animations
		raytracer.preprocessAnimation(root, frame);

		// Preprocess lights
		for (Light* light : lights)
		{
			light->animateLight(frame);
		}

		// Multithread  by passing a slice of the image to a thread
		int numThreads = std::min(
			static_cast<unsigned int>(16), 
			std::thread::hardware_concurrency()
		);
		int threadWidth = nx / numThreads;
		std::vector<std::thread> threads;

#ifdef DEBUG_LOGS
		std::cout << "----- Starting render for frame: " << frame << " -----" << std::endl;
		std::cout << "----- Using " << numThreads << " threads -----" << std::endl;
#endif

		for (int i = 0; i < numThreads; i++)
		{
			int startWidth = i * threadWidth;
			int endWidth = (i == (numThreads - 1)) ? ny : (i + 1) * threadWidth;
			threads.emplace_back(&RayTracer::render, 
				raytracer, frame, i, startWidth, endWidth);
		}

		// Wait for threads to finish
		for (std::thread& thread : threads)
		{
			thread.join();
		}

#ifdef DEBUG_LOGS
		std::cout << "----- All threads completed -----" << std::endl;
#endif

		// After each frame  done rendering, reset animations to default
		raytracer.resetAnimation(root);
		for (Light* light : lights)
		{
			light->resetAnimation();
		}

		// Save image to disk
		std::stringstream fileNum;
		if (numFrames > 1 || startFrame != 0)
		{
			fileNum << "_" << std::setw(4) << std::setfill('0') << frame;
		}
		fileNum << ".png";
		image.savePng(fileName + fileNum.str());
	}

  std::cout << "A5_Render(\n" <<
		  "\t" << *root <<
          "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
          "\t" << "eye:  " << glm::to_string(eye) << std::endl <<
		  "\t" << "view: " << glm::to_string(view) << std::endl <<
		  "\t" << "up:   " << glm::to_string(up) << std::endl <<
		  "\t" << "fovy: " << fovy << std::endl <<
          "\t" << "ambient: " << glm::to_string(ambient) << std::endl <<
		  "\t" << "lights{" << std::endl;

	for(const Light * light : lights) {
		std::cout << "\t\t" <<  *light << std::endl;
	}
	std::cout << "\t}" << std::endl;
	std:: cout <<")" << std::endl;
}
