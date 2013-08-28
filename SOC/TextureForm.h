#pragma once

#include "Size.h"

namespace Rendering
{
	namespace Texture
	{
		class TextureForm
		{
		protected:
			Common::Size<int> size;

		public:
			TextureForm(void)
			{
			}
			~TextureForm(void)
			{
			}

		public:
			virtual bool Create(const char *path) = 0;


		public:
			Common::Size<int> GetSize()
			{
				return size;
			}
		};

	}
}