// Fragment shader for per-pixel Phong interpolation and shading.

// The "varying" keyword means that the parameter's value is interpolated
// between the nearby vertices.
varying vec3 N;
varying vec3 v;

uniform float brightness;
uniform sampler2D colorMap;

void main()
{
	vec3 Normal = 2.0 * texture2D (colorMap, gl_TexCoord[0].st).rgb - 1.0;
	Normal = normalize(Normal + N);
	
    // The scene's ambient light.
    vec4 ambient = gl_LightModel.ambient * gl_FrontMaterial.ambient;

	// The normal vectors is generally not normalized after being
	// interpolated across a triangle.  Here we normalize it.
	//vec3 Normal = normalize(N);

	// Since the vertex is in eye space, the direction to the
	// viewer is simply the normalized vector from v to the
	// origin.
	vec3 Viewer = -normalize(v);

	// Get the lighting direction and normalize it.
	vec3 DirLight = normalize(gl_LightSource[0].position.xyz);
	vec3 PntLight = normalize(gl_LightSource[1].position.xyz - v);

	// Compute halfway vector
	vec3 DirHalf = normalize(Viewer+DirLight);
	vec3 PntHalf = normalize(Viewer+PntLight);

	// Compute factor to prevent light leakage from below the
	// surface
	float DirB = 1.0;
	if(dot(Normal, DirLight)<0.0) DirB = 0.0;
	float PntB = 1.0;
	if(dot(Normal, PntLight)<0.0) PntB = 0.0;

	// Compute geometric terms of diffuse and specular
	float dirDiffuseShade = max(dot(Normal, DirLight), 0.0);
	float dirSpecularShade = 
	  DirB * pow(max(dot(DirHalf, Normal), 0.0), gl_FrontMaterial.shininess);
	float pntDiffuseShade = max(dot(Normal, PntLight), 0.0);
	float pntSpecularShade = 
	  PntB * pow(max(dot(PntHalf, Normal), 0.0), gl_FrontMaterial.shininess);

	// Compute product of geometric terms with material and
	// lighting values
	vec4 dirDiffuse = dirDiffuseShade * gl_FrontLightProduct[0].diffuse;
	vec4 dirSpecular = dirSpecularShade * gl_FrontLightProduct[0].specular;
	ambient += gl_FrontLightProduct[0].ambient;
	float distance = length(gl_LightSource[1].position.xyz - v);
	float attenuation = gl_LightSource[1].quadraticAttenuation * pow(distance, 2)
					  + gl_LightSource[1].linearAttenuation * distance
					  + gl_LightSource[1].constantAttenuation;
	vec4 pntDiffuse = pntDiffuseShade * gl_FrontLightProduct[1].diffuse / attenuation;
	vec4 pntSpecular = pntSpecularShade * gl_FrontLightProduct[1].specular / attenuation;
	ambient += gl_FrontLightProduct[1].ambient / attenuation;

	// Assign final color
	gl_FragColor = ambient + dirDiffuse + dirSpecular + pntDiffuse + pntSpecular + gl_FrontMaterial.emission;

}