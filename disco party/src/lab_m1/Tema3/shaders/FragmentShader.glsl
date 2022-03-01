#version 330

#define MAX_LIGHTS 50

// Input
in vec3 world_position;  // nu e normalizat
in vec3 world_normal;    // e normalizat
in vec2 texcoord;

// Texture uniform properties
uniform sampler2D disco_ball_texture;
uniform bool is_texture_moving;

// Light uniform properties
uniform vec3 eye_position;
uniform int num_lights;
uniform vec3 light_position[MAX_LIGHTS];
uniform vec3 light_direction[MAX_LIGHTS];
uniform vec3 light_color[MAX_LIGHTS];
uniform bool is_spotlight[MAX_LIGHTS];
uniform bool disco_light_active;
uniform bool floor_light_active;
uniform bool spotlight_active;

// Material uniform properties
uniform vec4 object_color;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform float material_ambient_light;

// Other uniform properties
uniform float elapsed_time;
uniform vec3 disco_ball_position;

// Output
layout(location = 0) out vec4 out_color;

bool is_disco_ball()
{
	return object_color.a == 0;
}


void main()
{
	vec4 tmp_out_color;

	// Calculam forma texturii de lumina din discoball pe toate obiectele ce ruleaza in acest shader 
	vec3 light_dir = world_position - disco_ball_position;

	float texcoordx = (1.0 / (2 * 3.14159)) * atan (light_dir.x, light_dir.z);
	float texcoordy = (1.0 / 3.14159) * acos (light_dir.y / length (light_dir));
	texcoordx += mod(elapsed_time / 10, 10);  // Rotim textura
	vec4 color_disco_ball_texture = vec4(texture(disco_ball_texture, vec2(texcoordx, texcoordy)).xyz, 1);

	if (is_disco_ball())
	{
		tmp_out_color = color_disco_ball_texture;

		// vec4 color contine (red, green, blue, alpha)
		if (tmp_out_color.a < 0.5f) 
		{
			discard;
		}
	}
	else
	{
		if (object_color.a < 1.0f)
		{
			out_color = object_color;
			return;
		}

		vec3 diffuse_light = vec3(0, 0, 0);
		float attenuation_factor = 0.0f;
		float light_intensity = 1.0f;
		vec3 specular_light = vec3(0, 0, 0);
		float receive_light = 0.0f;
		vec4 computed_light = vec4(0);

		for (int i = 0; i < num_lights; ++i)
		{
			receive_light = 0.0f;

			// L - incident light direction vector  (normalizat)
			// V - view direction vector            (normalizat)
			// R - reflection direction vector      (normalizat)
			vec3 L = normalize(light_position[i] - world_position);
			vec3 V = normalize(eye_position - world_position);
			vec3 R = normalize(reflect(-L, world_normal));

			float tmp_dif_light = material_kd * light_intensity * max(dot(world_normal, L), 0);
			if (tmp_dif_light > 0) receive_light = 1.0f;
			diffuse_light = tmp_dif_light * light_color[i];

			specular_light = material_ks * light_color[i] * receive_light * pow(max(dot(V, R), 0), material_shininess);

			if (is_spotlight[i])
			{
				float cutoff = 25.31f;
				float spot_light = dot(-L, light_direction[i]);
				float spot_light_limit = cos(cutoff);

				if (spot_light > spot_light_limit)
				{
					float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
					float light_att_factor = linear_att * linear_att;
					float light_range = 0.01f;
					attenuation_factor = light_att_factor * light_range;
				}
				else
				{
					attenuation_factor = 0.0f;
				}
			}
			else
			{
				float d = distance(light_position[i], world_position);

				// Cu cat intervalul de lumina este mai mic, cu atat material_kd este mai mare, pentru a vedea lumina.
				float light_range = 0.001f;
				attenuation_factor = light_range / (d * d);
			}

			computed_light += vec4(attenuation_factor * (diffuse_light + specular_light), 1);
		}

		float mix_percetange = 0.25f;  // Privind amestecul de textura cu culoarea obiectului initial
		if (!disco_light_active)
		{
			mix_percetange = 0.0f;
		}

		tmp_out_color = mix(object_color * (computed_light + vec4(material_ambient_light)), color_disco_ball_texture, mix_percetange);
	}

	out_color = tmp_out_color;
}
