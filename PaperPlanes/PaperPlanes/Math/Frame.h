#pragma once
#include "../glm/glm.hpp"

class Frame
{
public:
	Frame(void);
	Frame(const glm::vec3& position, const glm::vec3& forward, const glm::vec3& up);
	~Frame(void);

	void SetPosition(const glm::vec3& position) { m_Position = position; }
	void SetPosition(float x, float y, float z) { m_Position = glm::vec3(x, y, z); }
	const glm::vec3& GetPosition() const { return m_Position; }

	void SetForward(glm::vec3& forward) { m_Forward = forward; }
	void SetForward(float x, float y, float z) { m_Forward = glm::vec3(x, y, z); }
	const glm::vec3& GetForward() const { return m_Forward; }

	void SetUp(glm::vec3& up) { m_Up = up; }
	void SetUp(float x, float y, float z) { m_Up = glm::vec3(x, y, z); }
	const glm::vec3& GetUp() const { return m_Up; }

	const glm::mat4 GetMatrix() const;

	// Move the frame forward along forward vector.
	void MoveForward(float delta); 
	// Move the frame up along up vector.
	void MoveUp(float delta); 
	// Move the frame right (or left if negative delta).
	void MoveRight(float delta); 

	void RotateLocalX(const float delta);
	void RotateLocalY(const float delta);
	void RotateLocalZ(const float delta);

	void RotateGlobalX(const float delta);
	void RotateGlobalY(const float delta);
	void RotateGlobalZ(const float delta);

private:
	glm::mat4 m_Matrix;
	glm::vec3 m_Position;
	glm::vec3 m_Up;
	glm::vec3 m_Forward;
	glm::mat4 m_Rotation;	
};
