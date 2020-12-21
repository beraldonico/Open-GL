#version 120

varying vec4 texturepos;
varying vec3 normal;
varying vec3 vertex;
varying mat3 TBN;

void main() {

    texturepos = 10.0*vec4(gl_Vertex);//gl_MultiTexCoord0;

    vertex = vec3(gl_ModelViewMatrix * gl_Vertex);
    normal = normalize(gl_NormalMatrix * gl_Normal);

    vec3 N = normal;

    vec3 up = vec3(0,1,0);
    vec3 T = cross(up, N);
    vec3 B = cross(N, T);

    TBN = transpose(mat3(T,B,N));

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

