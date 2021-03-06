#version 120

varying vec4 texturepos;
varying vec3 normal;
varying vec3 vertex;

void main() {

    texturepos = 10.0*vec4(gl_Vertex);//gl_MultiTexCoord0;

    vertex = vec3(gl_ModelViewMatrix * gl_Vertex);
    normal = normalize(gl_NormalMatrix * gl_Normal);

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

