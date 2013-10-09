#pragma once

#include "RenderingMath.h"
#include <vector>
#include "Color.h"

namespace Rendering
{
	namespace Mesh
	{

		struct MeshFilterElements
		{
		public:
			typedef int count;

		public:
			SOC_Vector3 *vertices;
			SOC_Vector3 *normals;
			SOC_Vector3 *tangents;
			SOC_Vector3 *binomals;
			std::pair<count, SOC_Vector2**> texcoords;
			Color *colors;
			int numOfVertex;
			int numOfTriangle;
			std::pair<count, SOC_word*> indices;
			SOC_TRIANGLE type;
			bool isDynamic;

		public:
			MeshFilterElements(void);
			~MeshFilterElements(void);
		};
	}

}