#version 430 core

struct Light
{
	float ambientStrength;

	vec3 diffuseColor;
	float diffuseStrength;

	float specularStrength;
};

struct DirLight
{
	Light base;

	vec3 direction;
};

struct PointLight
{
	Light base;

	vec3 position;
	float constant, linear, exponent;	//Quadratic equation for attenuation
};




in vec2 texCoord;
in vec3 Normal;
in vec3 WorldPos;

out vec4 frag_colour;

uniform sampler2D texture0;

uniform DirLight dirLight;

#define NR_POINT_LIGHTS 3
uniform PointLight pLight[NR_POINT_LIGHTS];

uniform vec3 eyePos;
uniform float Shininess;

vec3 CalcDirLight(vec3 normal, vec3 viewDir)
{
	////////////calculate ambient//////////////////
	vec3 ambient = dirLight.base.diffuseColor * dirLight.base.ambientStrength;
	//////////////////////////////////////////////////////

	////////////calculate diffuse//////////////////
	vec3 lightDir = normalize(-dirLight.direction);
	float diffuseFactor = max(dot(normal, lightDir), 0.f);
	vec3 diffuse = dirLight.base.diffuseColor * dirLight.base.diffuseStrength * diffuseFactor;
	//////////////////////////////////////////////////////

	////////////calculate specular//////////////////
	vec3 specular = vec3(0.f, 0.f, 0.f);
	if(diffuseFactor > 0.f)
	{
		vec3 reflectDir = reflect(dirLight.direction, normal);
		float specularFactor = dot(viewDir, reflectDir);

		if(specularFactor > 0.f)
		{
			specularFactor = pow(specularFactor, Shininess);
			specular = dirLight.base.diffuseColor * dirLight.base.specularStrength * specularFactor;
		}
	}
	//////////////////////////////////////////////////////


	return (ambient + diffuse + specular);
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
	////////////calculate ambient//////////////////
	vec3 ambient = light.base.diffuseColor * light.base.ambientStrength;
	//////////////////////////////////////////////////////



	////////////calculate diffuse//////////////////
	//calculate the vector from this pixels surface to the light source
    vec3 surfaceToLight = light.position - WorldPos;
	float distanceSurfaceToLight = length(surfaceToLight);
	surfaceToLight = normalize(surfaceToLight);

	//Calculate diffuse and diffuseFactor
	float diffuseFactor = max(dot(normal, surfaceToLight), 0.f);
	vec3 diffuse = light.base.diffuseColor * light.base.diffuseStrength * diffuseFactor;
	//////////////////////////////////////////////////////


	////////////calculate specular//////////////////
	vec3 specular = vec3(0.f, 0.f, 0.f);
	if(diffuseFactor > 0.f)
	{
		//vec3 surfaceToEye = normalize(eyePos - WorldPos);
		vec3 reflectDir = reflect(-surfaceToLight, normal);

		float specularFactor = dot(viewDir, reflectDir);

		if(specularFactor > 0.f)
		{
			specularFactor = pow(specularFactor, Shininess);
			specular = light.base.diffuseColor * light.base.specularStrength * specularFactor;
		}
	}
	//////////////////////////////////////////////////////



	////////////calculate attenuation//////////////////
	float attenuation = 1.0 / (light.constant + 
							   light.linear * distanceSurfaceToLight + 
		 					   light.exponent * (distanceSurfaceToLight * distanceSurfaceToLight)); 

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	//////////////////////////////////////////////////////

	return (ambient + diffuse + specular);
}

void main()
{
	vec3 norm = normalize(Normal);
	vec3 resultLight = vec3(0.f, 0.f, 0.f);
	vec3 surfaceToEye = normalize(eyePos - WorldPos);

	//Calculate Directional light
	resultLight += CalcDirLight(norm, surfaceToEye);

	//Calculate All Point Light
	for(int i = 0; i < NR_POINT_LIGHTS; ++i)
	{
		resultLight += CalcPointLight(pLight[i], norm, surfaceToEye);
	}
		

	vec4 surfaceColor = texture(texture0, texCoord);									
	frag_colour = vec4(surfaceColor.rgb * resultLight, surfaceColor.a);
}