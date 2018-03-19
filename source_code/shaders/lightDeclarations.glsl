R"(

	struct DirectionalLight {
		vec3    	direction;
		vec4    	color;
	};

	struct PointLight {
		float 		power;
		vec4		color;
		vec3		position;
	};

	struct SpotLight {
		float 		power;
		vec4 		color;
		vec3		position;
		float		angleDegrees;
		vec3		direction;
	};

)";
