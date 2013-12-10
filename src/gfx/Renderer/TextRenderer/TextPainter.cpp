#include "TextPainter.hpp"

namespace GFX
{

	TextPainter::TextPainter(ShaderManager* shaderManager,UniformBufferManager* uniformBufferManager)
		: BasePainter(shaderManager, uniformBufferManager)
	{
	}

	TextPainter::~TextPainter()
	{
		delete(m_atlas48);
	}

	void TextPainter::Initialize(GLuint FBO, GLuint dummyVAO)
	{
		BasePainter::Initialize(FBO, dummyVAO);

		if (FT_Init_FreeType(&m_library))
		{
			std::cout << "Could not Initialize freetype library.\n";
		}

		/* Load a font */
		if (FT_New_Face(m_library, "assets/Fonts/Terminus.ttf", 0, &m_face))
		{
			assert(0 && "Could not	open font file in TextPainter.Initialize()\n");
		}

		m_shaderManager->CreateProgram("BasicText");
		m_shaderManager->LoadShader("shaders/text/Text.vertex", "BasicTextVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/text/Text.geometry", "BasicTextGS", GL_GEOMETRY_SHADER);
		m_shaderManager->LoadShader("shaders/text/Text.fragment", "BasicTextFS", GL_FRAGMENT_SHADER);

		m_shaderManager->AttachShader("BasicTextVS", "BasicText");
		m_shaderManager->AttachShader("BasicTextGS", "BasicText");
		m_shaderManager->AttachShader("BasicTextFS", "BasicText");

		m_shaderManager->LinkProgram("BasicText");
		m_shaderManager->UseProgram("BasicText");

		m_colorUniform = m_shaderManager->GetUniformLocation("BasicText", "fontColor");
		m_textureUniform = m_shaderManager->GetUniformLocation("BasicText", "textureAtlas");

		glGenBuffers(1, &m_textVBO);
		glGenVertexArrays(1, &m_textVAO);

		m_atlas48 = new FontTextureAtlas(m_face, 18, m_textureUniform);

	}

	void TextPainter::Render()
	{

		m_text = GetTextManager().GetText();

		for (unsigned int i = 0; i < m_text.size(); i++)
		{
			RenderText(
				m_text[i].text.c_str(),
				m_atlas48,
				m_text[i].posX,
				m_text[i].posY,
				m_text[i].sizeX,
				m_text[i].sizeY,
				m_text[i].color
				);
		}

		GetTextManager().ClearText();
	}

	void TextPainter::RenderText(const char* text, FontTextureAtlas* atlas, float x, float y, float sx, float sy, glm::vec4 color)
	{
		m_shaderManager->UseProgram("BasicText");

		const unsigned char* p;

		std::vector<TextVertex> coords;

		int c = 0;
		
		TextVertex tv;

		for (p = (const unsigned char*)text; *p; p++)
		{
			float x2 = x + atlas->characters[*p].bitmapLeft * sx;
			float y2 = -y - atlas->characters[*p].bitmapTop * sy;
			float w = atlas->characters[*p].bitmapWidth * sx;
			float h = atlas->characters[*p].bitmapHeight * sy;

			x += atlas->characters[*p].advanceX * sx;
			y += atlas->characters[*p].advanceY * sy;
			
			if (!w || !h)
				continue;

			tv.pos_dim = glm::vec4( x2, y2, w, h );
			tv.uv = glm::vec4(
				atlas->characters[*p].uvOffsetX,
				atlas->characters[*p].uvOffsetY,
				atlas->characters[*p].uvOffsetX + atlas->characters[*p].bitmapWidth / static_cast<float>(atlas->width),
				atlas->characters[*p].uvOffsetY + atlas->characters[*p].bitmapHeight / static_cast<float>(atlas->height)
				);
			coords.push_back(tv);
		}

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(atlas->textureUniform, 0);
		glBindTexture(GL_TEXTURE_2D, atlas->texture);

		m_shaderManager->SetUniform(1, color, m_colorUniform);

		glBindBuffer(GL_ARRAY_BUFFER, m_textVBO);
		glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(TextVertex), coords.data(), GL_DYNAMIC_DRAW);

		//Generate VAO
		glBindVertexArray(m_textVAO);

		//Position dimension
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (void*)0);

		//UV
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (void*)(4 * sizeof(float)));

		glBindVertexArray(m_textVAO);
		glDrawArrays(GL_POINTS, 0, coords.size());

		glDisableVertexAttribArray(0);
		m_shaderManager->ResetProgram();

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}
}
