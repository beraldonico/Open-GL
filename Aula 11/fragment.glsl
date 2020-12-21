#version 120

uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform samplerCube cubemap;

varying vec4 texturepos;
varying vec3 normal;
varying vec3 vertex;
varying vec3 vertex2;
varying mat3 TBN;

void main (void)
{
   vec4 texColor = texture2D(colorTexture, texturepos.st);

   vec4 texNormal = texture2D(normalTexture, texturepos.st);
   vec3 tNormal = vec3(texNormal)*2-1;

   tNormal = TBN*tNormal;

   //vec3 N = normalize(tNormal);
   vec3 N = normalize(normal);
   vec3 V = normalize(-vertex);

   vec3 L = normalize(vec3(gl_LightSource[0].position)-vertex);

   vec3 R = 2 * dot(L,N) * N - L;
   vec3 Rv = 2 * dot(V,N) * N - V;
   vec4 cubeColor = textureCube(cubemap, Rv);

   float fd = max(dot(L,N),0.0);
   float fs = pow(max(dot(R, V), 0.0), gl_FrontMaterial.shininess);

   vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
   vec4 diffuse = fd * texColor * gl_LightSource[0].diffuse;//gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
   vec4 specular = fs * cubeColor;//gl_FrontMaterial.specular * gl_LightSource[0].specular;

   gl_FragColor = cubeColor;
   //gl_FragColor = ambient + diffuse + specular;
   //gl_FragColor = texColor;
}
          
