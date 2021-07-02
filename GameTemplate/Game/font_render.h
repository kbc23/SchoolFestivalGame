#pragma once

class FontRender : public IGameObject
{
public:
    FontRender();
    ~FontRender();
    bool Start() override final;
    void Update() override final;
    void Render(RenderContext& renderContext) override final;


public:
    /**
     * @brief �������֐�
     * @param text �\������e�L�X�g
     * @param position ���W�B{ 0.0f,0.0f }�����S
     * @param color �J���[
     * @param rotation ��]
     * @param scale �g��
     * @param pivot �s�{�b�g�i��_�j
    */
    void Init(
        const wchar_t* text,
        const Vector2 position = { 0.0f,0.0f },
        const Vector4 color = { 1.0f,1.0f,1.0f,1.0f },
        const float rotation = 0.0f,
        const float scale = 1.0f,
        const Vector2 pivot = { 0.5f,0.5f }
    );


public: //Set�֐�
    /**
     * @brief �\������e�L�X�g��ݒ�
     * @param text �e�L�X�g
    */
    void SetText(const wchar_t* text)
    {
        swprintf_s(m_text, text);
    }

    /**
     * overload
     * @brief �\������e�L�X�g��ݒ�iint�^�j
     * @param num �e�L�X�g�iint�^�j
    */
    void SetText(const int num)
    {
        swprintf_s(m_text, L"%d", num);
    }

    /**
     * @brief �\������e�L�X�g�̈ʒu��ݒ�
     * @param vec �\������ʒu
    */
    void SetPosition(const Vector2 vec)
    {
        m_position = vec;
    }

    /**
     * @brief �\������e�L�X�g�̃s�{�b�g��ݒ�
     * @param vec �s�{�b�g
    */
    void SetPivot(const Vector2 vec)
    {
        m_pivot = vec;
    }


private: //constant
    static const int m_MAX_TEXT_SIZE = 256;


private: //data menber
    Font m_font;
    wchar_t m_text[m_MAX_TEXT_SIZE] = L""; //�\������e�L�X�g
    Vector2 m_position = { 0.0f,0.0f }; //�\������ꏊ
    Vector4 m_color = { 1.0f,1.0f,1.0f,1.0f }; //�����̃J���[�i���F�j
    float m_rotation = 0.0f; //��]
    float m_scale = 1.0f; //�g�嗦
    Vector2 m_pivot = { 0.5f,0.5f }; //�s�{�b�g�i��_�j


};