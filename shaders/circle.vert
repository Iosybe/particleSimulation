#version 430 core

layout(location = 0) in vec2 vertex;

struct Particle {
    vec2 pos;
    vec2 vel;
    float mass;
};

layout(std430, binding = 1) buffer particlesBuffer {
    Particle particles[];
};

uniform vec2 correction;
uniform vec2 transCorrection;

void main() {
    // Calculate in comp shader. sumVel?
    vec2 translation = (particles[gl_InstanceID].pos + transCorrection) * correction;
    vec2 scale = particles[gl_InstanceID].mass.xx * correction;

    gl_Position.xy = vertex * scale + translation;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;
}










// ---------------------- Debug stuff ----------------------

    // if (gl_InstanceID == 0 || gl_InstanceID == 1) {
    //     gl_Position.x = vertex.x * 0.01 + particles[1].pos[0];
    //     // gl_Position.x = vertex.x * 0.01 + gl_InstanceID / 2000.0;
    //     gl_Position.y = vertex.y * 0.01 + 0.8;
    //     gl_Position.z = 0.0;
    //     gl_Position.w = 1.0;
    // }
    // else if (gl_InstanceID == 2 || gl_InstanceID == 3) {
    //     // gl_Position.x = vertex.x * 0.01 + particles[gl_InstanceID].pos[0];
    //     // gl_Position.x = vertex.x * 0.01 + gl_InstanceID / 2000.0;
    //     gl_Position.y = vertex.y * 0.01 + 0.8;
    //     gl_Position.z = 0.0;
    //     gl_Position.w = 1.0;
    // }
    // else {
    //     gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
    // }

    // if (gl_InstanceID == 0) {
    //     // gl_Position.x = (vertex.x * 0.01 + float(gl_InstanceID % 32) * 0.05) - 0.80;
    //     // gl_Position.y = (vertex.y * 0.01 + float(gl_InstanceID / 32) * 0.05) - 0.80;
    //     gl_Position.x = vertex.x * 0.01 + particles[501].pos[0];
    //     gl_Position.y = vertex.y * 0.01 + 0.0;
    //     gl_Position.z = 0.0;
    //     gl_Position.w = 1.0;
    // }
    // else if (gl_InstanceID == 1) {
    //     gl_Position.x = vertex.x * 0.01 + particles[501].pos[1];
    //     gl_Position.y = vertex.y * 0.01 + 0.2;
    //     gl_Position.z = 0.0;
    //     gl_Position.w = 1.0;

    // }
    // else if (gl_InstanceID == 2) {
    //     gl_Position.x = vertex.x * 0.01 + particles[501].vel[0];
    //     gl_Position.y = vertex.y * 0.01 + 0.4;
    //     gl_Position.z = 0.0;
    //     gl_Position.w = 1.0;

    // }
    // else if (gl_InstanceID == 3) {
    //     gl_Position.x = vertex.x * 0.01 + particles[501].vel[1];
    //     gl_Position.y = vertex.y * 0.01 + 0.6;
    //     gl_Position.z = 0.0;
    //     gl_Position.w = 1.0;

    // }
    // else if (gl_InstanceID == 4) {
    //     gl_Position.x = vertex.x * 0.01 + particles[501].mass;
    //     gl_Position.y = vertex.y * 0.01 + 0.8;
    //     gl_Position.z = 0.0;
    //     gl_Position.w = 1.0;

    // }
    // else {
    //     gl_Position.x = vertex.x * scale.x + translation.x;
    //     gl_Position.y = vertex.y * scale.y + translation.y;
    //     gl_Position.z = 0.0;
    //     gl_Position.w = 1.0;
    // }

// void main() {
    // vec2 translation;
    // vec2 scale;

    // translation = (particles[gl_InstanceID].pos + transCorrection) * correction;
    // scale = particles[i].mass.xx * correction;
    // scale.x = particles[gl_InstanceID].mass * correction.x;
    // scale.y = particles[gl_InstanceID].mass * correction.y;

//     if (gl_InstanceID == 0) {
//         // gl_Position.x = (vertex.x * 0.01 + float(gl_InstanceID % 32) * 0.05) - 0.80;
//         // gl_Position.y = (vertex.y * 0.01 + float(gl_InstanceID / 32) * 0.05) - 0.80;
//         gl_Position.x = vertex.x * 0.01 + particles[1].mass;
//         gl_Position.y = vertex.y * 0.01 + 0.0;
//         gl_Position.z = 0.0;
//         gl_Position.w = 1.0;
//     }
//     else if (gl_InstanceID == 1) {
//         gl_Position.x = vertex.x * 0.01 + particles[1].pos[0];
//         gl_Position.y = vertex.y * 0.01 + 0.2;
//         gl_Position.z = 0.0;
//         gl_Position.w = 1.0;

//     }
//     else if (gl_InstanceID == 2) {
//         gl_Position.x = vertex.x * 0.01 + particles[1].pos[1];
//         gl_Position.y = vertex.y * 0.01 + 0.4;
//         gl_Position.z = 0.0;
//         gl_Position.w = 1.0;

//     }
//     else if (gl_InstanceID == 3) {
//         gl_Position.x = vertex.x * 0.01 + particles[1].vel[0];
//         gl_Position.y = vertex.y * 0.01 + 0.6;
//         gl_Position.z = 0.0;
//         gl_Position.w = 1.0;

//     }
//     else if (gl_InstanceID == 4) {
//         gl_Position.x = vertex.x * 0.01 + particles[1].vel[1];
//         gl_Position.y = vertex.y * 0.01 + 0.8;
//         gl_Position.z = 0.0;
//         gl_Position.w = 1.0;

//     }
//     else {
//         gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
//     }
// }