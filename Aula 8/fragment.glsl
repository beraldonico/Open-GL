#version 120

varying vec3 normal;
varying vec3 vertex; 

void main (void)
{
   vec3 N = normalize(normal);
   vec3 V = normalize(-vertex);

   vec3 L = normalize(vec3(gl_LightSource[0].position)-vertex);

   vec3 R = 2 * dot(L,N) * N - L;

   float fd = max(dot(L,N),0.0);
   float fs = pow(max(dot(R, V), 0.0), gl_FrontMaterial.shininess);

   vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
   vec4 diffuse = fd * gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
   vec4 specular = fs * gl_FrontMaterial.specular * gl_LightSource[0].specular;

   gl_FragColor = ambient + diffuse + specular;
}
          
