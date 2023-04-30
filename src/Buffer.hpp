#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

class Buffer
{
    protected:
        unsigned int ID;
        unsigned int type;
        virtual unsigned int getType() const = 0;
    public:
        ~Buffer();

        void bind() const;
        void unbind() const;
        void bindBase(GLuint binding) const;

};

class IndexBuffer: public Buffer
{
    private:
        unsigned int count;
        virtual unsigned int getType() const;
    public:
        IndexBuffer(const GLuint* data, unsigned int count);
        unsigned int getCount() const;
};

class VertexBuffer: public Buffer
{
    private:
        virtual unsigned int getType() const;
    public:
        VertexBuffer(const GLfloat* data, unsigned int size);
};