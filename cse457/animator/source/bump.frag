// Fragment shader for per-pixel Phong interpolation and shading.

// The "varying" keyword means that the parameter's value is interpolated
// between the nearby vertices.
varying vec3 N;
varying vec3 v;
varying vec4 VertexUV;

uniform float brightness;
uniform sampler2D colorMap;

mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv)
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );
 
    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
 
    // construct a scale-invariant frame 
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}
 
vec3 perturb_normal( vec3 N, vec3 V, vec2 texcoord )
{
    // assume N, the interpolated vertex normal and 
    // V, the view vector (vertex to eye)
   vec3 map = texture(colorMap, texcoord ).xyz;
   map = map * 255./127. - 128./127.;
    mat3 TBN = cotangent_frame(N, -V, texcoord);
    return normalize(TBN * map);
}

void main()
{
	vec3 PntLight = normalize(gl_LightSource[1].position.xyz - v);
	vec3 Viewer = -normalize(v);
	vec3 Normal = normalize(N);

	vec2 uv = VertexUV.xy;
	Normal = perturb_normal(Normal, Viewer, uv);
	
    // The scene's ambient light.
    vec4 ambient = gl_LightModel.ambient * gl_FrontMaterial.ambient;

	// The normal vectors is generally not normalized after being
	// interpolated across a triangle.  Here we normalize it.
	//vec3 Normal = normalize(N);

	// Since the vertex is in eye space, the direction to the
	// viewer is simply the normalized vector from v to the
	// origin.
	

	// Get the lighting direction and normalize it.
	vec3 DirLight = normalize(gl_LightSource[0].position.xyz);
	

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
	float attenuation = gl_LightSource[1].quadraticAttenuation * pow(distance, 2.0)
					  + gl_LightSource[1].linearAttenuation * distance
					  + gl_LightSource[1].constantAttenuation;
	vec4 pntDiffuse = pntDiffuseShade * gl_FrontLightProduct[1].diffuse / attenuation;
	vec4 pntSpecular = pntSpecularShade * gl_FrontLightProduct[1].specular / attenuation;
	ambient += gl_FrontLightProduct[1].ambient / attenuation;

	// Assign final color
	gl_FragColor = ambient + dirDiffuse + dirSpecular + pntDiffuse + pntSpecular + gl_FrontMaterial.emission;

}