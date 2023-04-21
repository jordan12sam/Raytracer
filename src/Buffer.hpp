#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

class Buffer
{
    protected:
        unsigned int rendererID;
        unsigned int type;
        virtual unsigned int getType() const = 0;
    public:
        ~Buffer();

        void bind() const;
        void unbind() const;

};

class IndexBuffer: public Buffer
{
    private:
        unsigned int count;
        virtual unsigned int getType() const;
    public:
        IndexBuffer(const std::vector<int> data, unsigned int count);
        unsigned int getCount() const;
};

class VertexBuffer: public Buffer
{
    private:
        virtual unsigned int getType() const;
    public:
        VertexBuffer(const std::vector<float> data, unsigned int size);
};