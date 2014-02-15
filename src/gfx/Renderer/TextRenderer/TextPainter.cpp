#include "TextPainter.hpp"

namespace GFX
{

	TextPainter::TextPainter(ShaderManager* shaderManager,UniformBufferManager* uniformBufferManager)
		: BasePainter(shaderManager, uniformBufferManager)
	{
	}

	TextPainter::~TextPainter()
	{

	}

	void TextPainter::Initialize(GLuint FBO, GLuint dummyVAO)
	{
		BasePainter::Initialize(FBO, dummyVAO);

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

        glUniform1i(m_textureUniform, 0);

		glGenBuffers(1, &m_textVBO);
		glGenVertexArrays(1, &m_textVAO);
	}

	void TextPainter::Render(int screenWidth, int screenHeight)
	{
		m_text = GetTextManager().GetText();

		for (unsigned int i = 0; i < m_text.size(); i++)
		{
			RenderText(
				m_text[i].m_text.c_str(),
                m_text[i].m_fontData,
				((2 * m_text[i].m_posX) / static_cast<float>(screenWidth)) - 1.0f,
				((2 * m_text[i].m_posY) / static_cast<float>(screenHeight)) - 1.0f,
				m_text[i].m_sizeX * (2 / static_cast<float>(screenWidth)),
				-m_text[i].m_sizeY * (2 / static_cast<float>(screenHeight)),
				m_text[i].m_color
				);
		}

		GetTextManager().ClearText();
	}

	void TextPainter::RenderText(const char* text, FontData* fontData, float x, float y, float sx, float sy, glm::vec4 color)
	{
		m_shaderManager->UseProgram("BasicText");

		const unsigned char* p;

		std::vector<TextVertex> coords;
		
		TextVertex tv;

		for (p = (const unsigned char*)text; *p; p++)
		{
			float x2 = x + fontData->characters[*p].bitMapLeft * sx;
			float y2 = -y - fontData->characters[*p].bitMapTop * sy;
			float w = fontData->characters[*p].bitMapWidth * sx;
			float h = fontData->characters[*p].bitMapHeight * sy;

			x += fontData->characters[*p].advanceX * sx;
			y += fontData->characters[*p].advanceY * sy;
			
			if (!w || !h)
				continue;

			tv.pos_dim = glm::vec4( x2, y2, w, h );
			tv.uv = glm::vec4(
				fontData->characters[*p].uvOffSetX,
				fontData->characters[*p].uvOffSetY,
				fontData->characters[*p].uvOffSetX + fontData->characters[*p].bitMapWidth / static_cast<float>(fontData->atlasWidth),
				fontData->characters[*p].uvOffSetY + fontData->characters[*p].bitMapHeight / static_cast<float>(fontData->atlasHeight)
				);
			coords.push_back(tv);
		}

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glActiveTexture(GL_TEXTURE0);
		//glUniform1i(atlas->textureUniform, 0);
		glBindTexture(GL_TEXTURE_2D, fontData->fontAtlas);

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
		glDisableVertexAttribArray(1);
		glBindTexture(GL_TEXTURE_2D, 0);
		m_shaderManager->ResetProgram();

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

	}
}
