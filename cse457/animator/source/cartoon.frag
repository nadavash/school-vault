// Cartoon fragment shader.

varying vec3 N;
varying vec3 v;

vec4 DiscreteColor(vec3 halfLight, gl_LightProducts light)
{
	float intensity = dot(halfLight,N);
	vec4 color = (light.specular + light.diffuse) / 2.0;
	if (intensity > 0.99)
		color = vec4(1, 1, 1, 1);
	if (intensity > 0.9)
		color *= 0.9;
	else if (intensity > 0.3)
		color *= 0.65;
	else if (intensity > 0.0)
		color *= 0.1;
	else
		color *= 0.0;

	return color;
}

void main()
{
	vec3 normal = normalize(N);
	vec3 viewer = -normalize(v);

	vec3 dirLight = normalize(gl_LightSource[0].position.xyz);
	vec3 pntLight = normalize(gl_LightSource[1].position.xyz - v);

	// Compute halfway vector
	vec3 dirHalf = normalize(viewer+dirLight);
	vec3 pntHalf = normalize(viewer+pntLight);

	// Compute factor to prevent light leakage from below the
	// surface
	float dirB = 1.0;
	if(dot(normal, dirLight)<0.0) dirB = 0.0;
	float pntB = 1.0;
	if(dot(normal, pntLight)<0.0) pntB = 0.0;


	vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 color2 = vec4(0.0, 0.0, 0.0, 0.0);
	if (dot(normal, viewer) > 0.5) {
		color = DiscreteColor(dirHalf, gl_FrontLightProduct[0]);
	}
	if (dot(normal, viewer) > 0.5) {
		color2 = DiscreteColor(pntHalf, gl_FrontLightProduct[1]);
	}
	gl_FragColor += color + color2 + vec4(0.2,0.2,0.2, 1);

}