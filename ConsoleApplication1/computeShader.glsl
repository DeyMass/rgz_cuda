#version 430 core

layout( local_size_x = 2, local_size_y = 2 ) in;
layout(std430, binding = 80) buffer shader_data{
	float a[16];
};

void main()
{
	uint idx = gl_GlobalInvocationID.x;
	a[idx]++;
}