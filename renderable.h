#pragma once

//Represents something that can be rendered
class Renderable {
public:
	virtual void bind() = 0;
	virtual void render() = 0;
};