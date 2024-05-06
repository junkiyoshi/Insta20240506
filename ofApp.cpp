#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(39);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	auto ico_sphere = ofIcoSpherePrimitive(250, 5);
	this->mesh = ico_sphere.getMesh();

	for (int i = 0; i < this->mesh.getNumVertices(); i++) {

		this->param_list.push_back(0);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->draw_mesh = this->mesh;
	this->frame_mesh = this->mesh;

	for (int k = 0; k < 40; k++) {

		auto noise_seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));

		auto rotation_x = glm::rotate(glm::mat4(), ofMap(ofNoise(noise_seed.x, ofGetFrameNum() * 0.002), 0, 1, -PI * 4, PI * 4), glm::vec3(1, 0, 0));
		auto rotation_y = glm::rotate(glm::mat4(), ofMap(ofNoise(noise_seed.y, ofGetFrameNum() * 0.002), 0, 1, -PI * 4, PI * 4), glm::vec3(0, 1, 0));
		auto rotation_z = glm::rotate(glm::mat4(), ofMap(ofNoise(noise_seed.z, ofGetFrameNum() * 0.002), 0, 1, -PI * 4, PI * 4), glm::vec3(0, 0, 1));

		glm::vec3 location = glm::vec4(0, 0, 250, 0) * rotation_z * rotation_y * rotation_x;

		for (int i = 0; i < this->draw_mesh.getNumVertices(); i++) {

			auto distance = glm::distance(location, this->draw_mesh.getVertex(i));
			if (distance < 80) {

				this->param_list[i] = 50;
			}
		}
	}

	ofColor draw_color;
	for (int i = 0; i < this->draw_mesh.getNumVertices(); i++) {

		glm::vec3 vertex = this->draw_mesh.getVertex(i);
		vertex = glm::normalize(vertex) * (250 - ofMap(this->param_list[i], 50, 0, 100, 0));

		this->draw_mesh.setVertex(i, vertex);
		this->frame_mesh.setVertex(i, vertex);

		draw_color.setHsb(ofMap(this->param_list[i], 50, 0, 190, 255), 255, 255);

		this->draw_mesh.addColor(draw_color);
		this->frame_mesh.addColor(ofColor(200));
	}

	for (int i = 0; i < this->param_list.size(); i++) {

		this->param_list[i] -= 3;
		if (this->param_list[i] < 0) { this->param_list[i] = 0; }
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(ofGetFrameNum() * 2);
	ofRotateY(ofGetFrameNum());

	this->draw_mesh.draw();
	this->frame_mesh.drawWireframe();

	this->cam.end();

	/*
	int start = 300;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}