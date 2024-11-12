#version 330 core

in vec4 ourColor;
out vec4 FragColor;

uniform vec2 resolution; 
uniform float time;    
uniform float speed;

void main() {
    vec2 offset = vec2(-0.0235, 0.9794);  
    vec2 pitch = vec2(50.0, 50.0);            

    vec2 normalizedCoords = gl_FragCoord.xy / resolution;

    float scaleFactor = 10000.0;
    float offX = (scaleFactor * offset.x) + gl_FragCoord.x;
    float offY = (scaleFactor * offset.y) + (resolution.y - gl_FragCoord.y) - (time * speed);

    if (int(mod(offX, pitch.x)) == 0 || int(mod(offY, pitch.y)) == 0) {
        FragColor = vec4(0.5, 0.5, 0.5, 1.0);  
    } else {
        FragColor = ourColor;  
    }
}
