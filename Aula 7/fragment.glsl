#version 120

varying vec3 normal;
varying vec3 vertex; 

void main (void)
{
   vec3 V = normalize(-vertex);

   vec3 L = normalize(vec3(gl_LightSource[0].position)-vertex);

   vec3 R = 2 * dot(L,normal) * normal - L;

   float fs = pow(max(dot(R, V), 0.0), gl_FrontMaterial.shininess);

   float fd = max(dot(L,normal),0.0);

   gl_FragColor = gl_LightSource[0].ambient + fd * gl_LightSource[0].diffuse + fs * gl_LightSource[0].specular;
}
          
