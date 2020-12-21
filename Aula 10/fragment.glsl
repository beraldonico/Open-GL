#version 120

uniform sampler2D colorTexture;
uniform sampler2D normalTexture;

varying vec4 texturepos;
varying vec3 normal;
varying vec3 vertex;
varying mat3 TBN;

void main (void)
{
   vec4 texColor = texture2D(colorTexture, texturepos.st);

   vec4 texNormal = texture2D(normalTexture, texturepos.st);
   vec3 tNormal = vec3(texNormal)*2-1;

   tNormal = TBN*tNormal;

   vec3 N = normalize(tNormal);
   vec3 V = normalize(-vertex);

   vec3 L = normalize(vec3(gl_LightSource[0].position)-vertex);

   vec3 R = 2 * dot(L,N) * N - L;

   float fd = max(dot(L,N),0.0);
   float fs = pow(max(dot(R, V), 0.0), gl_FrontMaterial.shininess);

   vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
   vec4 diffuse = fd * texColor * gl_LightSource[0].diffuse;//gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
   vec4 specular = fs * gl_FrontMaterial.specular * gl_LightSource[0].specular;

   gl_FragColor = ambient + diffuse + specular;
   //gl_FragColor = texColor;
}
          
