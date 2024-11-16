#version 330 core

in vec4 ourColor;
out vec4 FragColor;

uniform vec2 resolution; 
uniform float time;    
uniform float speed;

#define SnowCount 5

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

float rand(float x) { return rand(vec2(x, 1.0)); }

float snow(vec3 pos, vec2 uv, float o) {
    vec2 d = (pos.xy - uv);
    float a = atan(d.y, d.x) + sin(time * speed + o) * 10.0;
    float dist = d.x * d.x + d.y * d.y;

    if (dist < pos.z / 400.0) {
        float col = 0.0;
        if (sin(a * 8.0) < 0.0) { col = 1.0; }
        if (dist < pos.z / 800.0) { col += 1.0; }
        return col * pos.z;
    }

    return 0.0;
}

float col(vec2 c) {
    float color = 0.0;

    for (int i = 0; i < SnowCount; ++i) {
        float o = rand(float(i) / 3.0) * float(SnowCount) * 2.0;
        float z = rand(float(i) + 13.0);
        float x = 1.8 - (1.8 * 2.0) * (rand(floor((time * ((z + 1.0) / 2.0) + o) / 2.0)) + sin(time * o / 1000.0) / 10.0);
        float y = 1.0 - mod((time * ((z + 1.0) / 2.0)) + o, 2.0);

        color += snow(vec3(x, y, z), c, o);
    }

    if (c.y < sin(c.x * 4.0) / 60.0 - 0.8) {
        color = -c.y + sin(c.x * 4.0) / 60.0;
    }

    return color;
}

void main() {
    vec2 uv = gl_FragCoord.xy / resolution;
    uv = uv * 2.0 - 1.0;
    uv.x *= resolution.x / resolution.y;

    // Scala il tempo in base alla velocità specificata
    float scaledTime = time / 4.0;

    // Colore di sfondo azzurro-grigio
    vec3 backgroundColor = vec3(0.6, 0.7, 0.8); // Colore azzurro-grigio (rgb: 0.6, 0.7, 0.8)

    // Calcola il colore dei fiocchi
    float snowColor = col(uv);
    vec3 finalColor = mix(backgroundColor, vec3(1.0), snowColor); // Fiocchi bianchi su sfondo azzurro-grigio

    FragColor = vec4(finalColor, 1.0);
}
