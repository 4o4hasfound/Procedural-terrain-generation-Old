#version 330 core
out vec4 FragColor; 

in vec3 FragPos;
in vec3 worldPos;
in vec3 Normal;
in vec3 ourColor;
in vec2 TexCoord;
in mat3 TBN;
flat in float snowHeight;
flat in float grassHeight;
flat in float waterHeight;

uniform sampler2D Snow;
uniform sampler2D SnowNor;
uniform sampler2D Grass;
uniform sampler2D GrassNor;
uniform sampler2D Dirt;
uniform sampler2D DirtNor;
uniform sampler2D Sand;
uniform sampler2D SandNor;

uniform vec3 dirLight;
uniform bool blinn;
uniform float shininess;
uniform vec3 viewPos;

uniform bool useLighting;
uniform bool useColoring;
uniform bool useTextures;
uniform bool useNormalMapping;

const int octaves = 13;
const float gDispFactor = 20.0;      
const float freq = 0.01;
const float power = 3.0;

uint hash(uint x) {
	x += (x << 10u);
	x ^= (x >> 6u);
	x += (x << 3u);
	x ^= (x >> 11u);
	x += (x << 15u);
	return x;
}

uint hash( uvec2 v ) { return hash( v.x ^ hash(v.y)                         ); }

float floatConstruct( uint m ) {
    const uint ieeeMantissa = 0x007FFFFFu; // binary32 mantissa bitmask
    const uint ieeeOne      = 0x3F800000u; // 1.0 in IEEE binary32

    m &= ieeeMantissa;                     // Keep only mantissa bits (fractional part)
    m |= ieeeOne;                          // Add fractional part to 1.0

    float  f = uintBitsToFloat( m );       // Range [1:2]
    return f - 1.0;                        // Range [0:1]
}
float Noise(float x, float y) { return floatConstruct(hash(floatBitsToUint(vec2(x,y)))); }
float SmoothedNoise(int x, int y) {
	float corners = (Noise(x - 1, y - 1) + Noise(x + 1, y - 1) +
		Noise(x - 1, y + 1) + Noise(x + 1, y + 1)) / 16,
		sides = (Noise(x - 1, y) + Noise(x + 1, y) + Noise(x, y - 1) +
			Noise(x, y + 1)) / 8,
		center = Noise(x, y) / 4;
	return corners + sides + center;
}

float Interpolate(float a, float b, float x) {
	float ft = x * 3.1415927,
		f = (1 - cos(ft)) * 0.5;
	return  a * (1 - f) + b * f;
}

float InterpolatedNoise(float x, float y) {
	int integer_X = int(floor(x));
	float fractional_X = x - integer_X;
	int integer_Y = int(floor(y));
	float fractional_Y = y - integer_Y;

	float v1 = SmoothedNoise(integer_X, integer_Y),
		v2 = SmoothedNoise(integer_X + 1, integer_Y),
		v3 = SmoothedNoise(integer_X, integer_Y + 1),
		v4 = SmoothedNoise(integer_X + 1, integer_Y + 1),
		i1 = Interpolate(v1, v2, fractional_X),
		i2 = Interpolate(v3, v4, fractional_X);
	return Interpolate(i1, i2, fractional_Y);
}

float persistence = 0.5;

float noise(float x, float y, int octwaves) {
	float total = 0,
		frequency = pow(2, octwaves),
		amplitude = 1;
	for (int i = 0; i < octwaves; ++i) {
		frequency /= 2;
		amplitude *= persistence;
		total += InterpolatedNoise(x / frequency, y / frequency) * amplitude;
	}
	return total / frequency;
}


void main()
{
    vec3 ourColor;
    vec3 normal;
    float trans = 20.0;
    float snowHeight = snowHeight + trans*(1-2*noise(worldPos.x+101, worldPos.z+101, 5)-1)*2;
    float grassHeight = grassHeight + trans*(1-2*noise(worldPos.x+201, worldPos.z+201, 5)-1)*2;
    float sandHeightTop = waterHeight + trans*(2*noise(worldPos.x+307, worldPos.z+307, 5)-1)+trans/2;
    vec3 snowColor = vec3(1.0, 0.98, 0.98);
    vec3 grassColor = vec3(0.337, 0.49, 0.274);
    vec3 dirtColor = vec3(0.607, 0.462, 0.325);
    vec3 sandColor = vec3(0.39, 0.36, 0.26);

    if(worldPos.y > snowHeight-trans && worldPos.y < snowHeight + trans){
        float blend = (worldPos.y-snowHeight+trans)/(trans*2);
        if(useTextures) ourColor = mix(vec3(texture(Grass, TexCoord)),vec3(texture(Snow, TexCoord)), blend);
        else if(useColoring) ourColor = mix(grassColor, snowColor, blend);
        else ourColor = vec3(1.0,1.0,1.0);
        if(useNormalMapping) normal = mix(normalize(TBN*(texture(GrassNor, TexCoord).rgb*2.0-1.0)), normalize(TBN*(texture(SnowNor, TexCoord).rgb*2.0-1.0)), blend);
        else normal = Normal;
    }
    else if (worldPos.y > snowHeight+trans){
        if(useTextures) ourColor = vec3(texture(Snow, TexCoord));
        else if(useColoring) ourColor = snowColor;
        else ourColor = vec3(1.0,1.0,1.0);
        if(useNormalMapping) normal = normalize(TBN*(texture(SnowNor, TexCoord).rgb*2.0-1.0));
        else normal = Normal;
    }
    else if(worldPos.y > grassHeight-trans && worldPos.y < grassHeight + trans){
        float blend = (worldPos.y-grassHeight+trans)/(trans*2);
        if(useTextures) ourColor = mix(vec3(texture(Dirt, TexCoord)),vec3(texture(Grass, TexCoord)), blend);
        else if(useColoring) ourColor = mix(dirtColor, grassColor, blend);
        else ourColor = vec3(1.0,1.0,1.0);
        if(useNormalMapping) normal = mix(normalize(TBN*(texture(DirtNor, TexCoord).rgb*2.0-1.0)), normalize(TBN*(texture(GrassNor, TexCoord).rgb*2.0-1.0)), blend);
        else normal = Normal;
    }
    else if (worldPos.y > grassHeight+trans){
        if(useTextures) ourColor = vec3(texture(Grass, TexCoord));
        else if(useColoring) ourColor = grassColor;
        else ourColor = vec3(1.0,1.0,1.0);
        if(useNormalMapping) normal = normalize(TBN*(texture(GrassNor, TexCoord).rgb*2.0-1.0));
        else normal = Normal;
    }
    else if(worldPos.y > waterHeight && worldPos.y < sandHeightTop){
        float blend = (worldPos.y-waterHeight)/(sandHeightTop-waterHeight);
        if(useTextures) ourColor = mix(vec3(texture(Sand, TexCoord)),vec3(texture(Dirt, TexCoord)), blend);
        else if(useColoring) ourColor = mix(sandColor, dirtColor, blend);
        else ourColor = vec3(1.0,1.0,1.0);
        if(useNormalMapping) normal = mix(normalize(TBN*(texture(SandNor, TexCoord).rgb*2.0-1.0)), normalize(TBN*(texture(DirtNor, TexCoord).rgb*2.0-1.0)), blend);
        else normal = Normal;
    }
    else if(worldPos.y > sandHeightTop){
        if(useTextures) ourColor = vec3(texture(Dirt, TexCoord));
        else if(useColoring) ourColor = dirtColor;
        else ourColor = vec3(1.0,1.0,1.0);
        if(useNormalMapping) normal = normalize(TBN*(texture(DirtNor, TexCoord).rgb*2.0-1.0));
        else normal = Normal;
    }
    else{
        if(useTextures) ourColor = vec3(texture(Sand, TexCoord));
        else if(useColoring) ourColor = sandColor;
        else ourColor = vec3(1.0,1.0,1.0);
        if(useNormalMapping) normal = normalize(TBN*(texture(SandNor, TexCoord).rgb*2.0-1.0));
        else normal = Normal;
    }
    if(useLighting) {
        if(!useNormalMapping) normal = Normal;
        float ambientStrength = 0.1;
        vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
        vec3 ambient = ambientStrength * lightColor;

        vec3 norm = normalize(normal);

        float diff = max(dot(norm, dirLight), 0.0);
        vec3 diffuse = diff * lightColor;
    
        vec3 result = ambient + diffuse * ourColor;
        FragColor = vec4(result, 1.0);
    }
    else{
        FragColor = vec4(ourColor, 1.0);
    }
}
