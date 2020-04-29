R"zzz(
#version 330 core
uniform vec4 light_position;
uniform vec3 camera_position;

uniform vec3 joint_trans[128];
uniform vec4 joint_rot[128];
//uniform vec3 joint_axis[128];
//uniform float joint_angle[128];

in int jid0;
in int jid1;
in float w0;
in vec3 vector_from_joint0;
in vec3 vector_from_joint1;
in vec4 normal;
in vec2 uv;
in vec4 vert;

out vec4 vs_light_direction;
out vec4 vs_normal;
out vec2 vs_uv;
out vec4 vs_camera_direction;

//struct DualQuat {
//	vec4 q0;
//	vec4 qe;
//};

vec3 qtransform(vec4 q, vec3 v) {
	return v + 2.0 * cross(cross(v, q.xyz) - q.w*v, q.xyz);
}
//
//DualQuat inverse(DualQuat quat1) {
//	vec4 reg = quat1.q0;
//	vec4 e = quat1.qe;
//	DualQuat ret = DualQuat(vec4(1.0 / reg[0], 0.0, 0.0, 0.0), vec4(-e[0] / (reg[0] * reg[0]), 0.0, 0.0, 0.0));
//	return ret;
//}
//
//DualQuat conjugate(DualQuat quat1) {
//	DualQuat ret = DualQuat(vec4(quat1.q0[0], -quat1.q0[1], -quat1.q0[2], -quat1.q0[3]), -1.0 * vec4(quat1.qe[0], -quat1.qe[1], -quat1.qe[2], -quat1.qe[3]));
//	return ret;
//}
//
//DualQuat norm(DualQuat quat1) {
//	vec4 q0 = quat1.q0;
//	vec4 qe = quat1.qe;
//	vec4 q0Norm = vec4(length(q0), 0.0, 0.0, 0.0);
//	vec4 newQe = vec4(dot(q0, qe) / length(q0), 0.0, 0.0, 0.0);
//	DualQuat ret = DualQuat(q0Norm, newQe);
//	return ret;
//}
//
//vec4 multiplyQ(vec4 q1, vec4 q2) {
//
//	float s0 = q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2] - q1[3] * q2[3];
//	float s1 = q1[0] * q2[1] + q1[1] * q2[0] + q1[2] * q2[3] - q1[3] * q2[2];
//	float s2 = q1[0] * q2[2] + q1[2] * q2[0] + q1[3] * q2[1] - q1[1] * q2[3];
//	float s3 = q1[0] * q2[3] + q1[3] * q2[0] + q1[1] * q2[2] - q1[2] * q2[1];
//	vec4 ret = vec4(s0, s1, s2, s3);
//	return ret;
//}
//
//DualQuat multiplyDQ(DualQuat quat1, DualQuat quat2) {
//	DualQuat ret = DualQuat(multiplyQ(quat1.q0, quat2.q0), multiplyQ(quat1.q0, quat2.qe) + multiplyQ(quat1.qe, quat2.q0));
//	return ret;
//}
//
//DualQuat addDQ(DualQuat quat1, DualQuat quat2) {
//	DualQuat ret = DualQuat(quat1.q0 + quat2.q0, quat1.qe + quat2.qe);
//	return ret;
//}
//
//DualQuat weightDQ(float w, DualQuat quat) {
//	DualQuat ret = DualQuat(w * quat.q0, w * quat.qe);
//	return ret;
//}
//
//
//
//DualQuat interpolatedTransform(float w, vec4 rot1, vec4 rot2, vec3 trans1, vec3 trans2, vec3 utrans1, vec3 utrans2) {
//	DualQuat rotation1 = DualQuat(rot1, vec4(0.0, 0.0, 0.0, 0.0));
//	DualQuat rotation2 = DualQuat(rot2, vec4(0.0, 0.0, 0.0, 0.0));
//	DualQuat trans11 = DualQuat(vec4(1.0, 0.0, 0.0, 0.0), vec4(0.0, trans1[0] / 2.0, trans1[1] / 2.0, trans1[2] / 2.0));
//	DualQuat trans12 = DualQuat(vec4(1.0, 0.0, 0.0, 0.0), vec4(0.0, -utrans1[0] / 2.0, -utrans1[1] / 2.0, -utrans1[2] / 2.0));
//	DualQuat trans21 = DualQuat(vec4(1.0, 0.0, 0.0, 0.0), vec4(0.0, trans2[0] / 2.0, trans2[1] / 2.0, utrans2[2] / 2.0));
//	DualQuat trans22 = DualQuat(vec4(1.0, 0.0, 0.0, 0.0), vec4(0.0, -utrans2[0] / 2.0, -utrans2[1] / 2.0, -utrans2[2] / 2.0));
//
//	DualQuat quat1 = multiplyDQ(trans11, multiplyDQ(rotation1, trans12));
//	DualQuat quat2 = multiplyDQ(trans21, multiplyDQ(rotation2, trans22));
//
//
//	DualQuat weightedSum = addDQ(weightDQ(w, quat1), weightDQ(1.0 - w, quat2));
//	DualQuat ret = multiplyDQ(weightedSum,inverse(norm(weightedSum)));
//
//
//	return ret;
//}
//
//
//


void main() {
	// FIXME: Implement linear skinning here

	if (jid1 != -1) {
 	//Gets the joint positions based off of the fact that vector = vertex - joint
//		vec3 utrans1 = vec3(vert) - vector_from_joint0;
//		vec3 utrans2 = vec3(vert) - vector_from_joint1;
//		vec4 rotation1 = vec4(joint_rot[jid0][3], joint_rot[jid0][0], joint_rot[jid0][1], joint_rot[jid0][2]);
//		vec4 rotation2 = vec4(joint_rot[jid1][3], joint_rot[jid1][0], joint_rot[jid1][1], joint_rot[jid1][2]);
//
//	//Gets interpolated transform
//		DualQuat transform = interpolatedTransform(w0, rotation1, rotation2, joint_trans[jid0], joint_trans[jid1], utrans1, utrans2);
//	//Position = 1 + e(v_x i + v_y j + v_z z)
//		DualQuat position = DualQuat(vec4(1.0, 0.0, 0.0, 0.0), vec4(0.0, vert[0], vert[1], vert[2]));
//	//New position = q * position * q_conjugate
//
//		DualQuat result = multiplyDQ(transform, multiplyDQ(position, conjugate(transform)));
//		vec4 qe = result.qe;
//
//
//		//gl_Position = vec4(qe[1], qe[2], qe[3], 1.0);


		gl_Position = vec4(w0 * (qtransform(joint_rot[jid0], vector_from_joint0) + joint_trans[jid0]) + (1.0 - w0) * (qtransform(joint_rot[jid1], vector_from_joint1) + joint_trans[jid1]), 1);
	}

	else {
//		vec3 utrans1 = vec3(vert) - vector_from_joint0;
//		DualQuat trans11 = DualQuat(vec4(1.0, 0.0, 0.0, 0.0), vec4(0.0, joint_trans[jid0][0] / 2.0, joint_trans[jid0][1] / 2.0, joint_trans[jid0][2] / 2.0));
//		DualQuat trans12;
//		trans12.q0 = vec4(1.0, 0.0, 0.0, 0.0);
//		trans12.qe = vec4(0.0, -utrans1[0] / 2.0, -utrans1[1] / 2.0, -utrans1[2] / 2.0);
//
//		float angle = joint_angle[jid0] / 2.0;
//		vec3 axis = joint_axis[jid0];
//		DualQuat rotation1 = DualQuat(vec4(joint_rot[jid0][3], joint_rot[jid0][0], joint_rot[jid0][1], joint_rot[jid0][2]), vec4(0.0, 0.0, 0.0, 0.0));
//		DualQuat transform1 = multiplyDQ(trans11, multiplyDQ(rotation1, trans12));
//
//		DualQuat position = DualQuat(vec4(1.0, 0.0, 0.0, 0.0), vec4(0.0, vert[0], vert[1], vert[2]));
//		DualQuat result = multiplyDQ(transform1, multiplyDQ(position, conjugate(transform1)));
//
//
//		vec4 qe = result.qe;

		
		//gl_Position = vec4(qe[1], qe[2], qe[3], 1.0); 
		

		gl_Position = vec4(qtransform(joint_rot[jid0], vector_from_joint0) + joint_trans[jid0], 1.0);
	}


	vs_normal = normal;
	vs_light_direction = light_position - gl_Position;
	vs_camera_direction = vec4(camera_position, 1.0) - gl_Position;
	vs_uv = uv;


}
)zzz"
