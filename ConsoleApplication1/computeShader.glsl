#version 430 core

layout( local_size_x = 4, local_size_y = 4 ) in;
layout(std430, binding = 80) buffer shader_data{
	float A[16];
	float B[16];
	float C[16];
};

uint N = 4;

void main()
{
	uint idx = gl_LocalInvocationID.x + gl_LocalInvocationID.y * N;
	uint Nline = gl_LocalInvocationID.y;
	uint Ncols = gl_LocalInvocationID.x % N;
	float sum = 0.0;
	for (uint k = 0; k < N; k++) {
		sum = sum + A[k + Nline * N] * B[Ncols + k * N];
	}
	//C[Ncols] = 450000;
	C[idx] = sum;

	uint k = 51;
	memoryBarrier();
	memoryBarrier();
}