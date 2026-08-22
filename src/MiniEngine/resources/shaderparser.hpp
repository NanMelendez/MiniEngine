#ifndef __MINIENGINE_SHADER_PARSER__
#define __MINIENGINE_SHADER_PARSER__

#include "../wrappers/shader.hpp"

/*
Basic structure of a custom shader file:
=========================================
Shader "MyShader" {
    Properties {
        // ...
    }
    Program {
        Stage vertex {
            // ...
        }
        Stage fragment {
            // ...
        }
    }
}
*/

namespace MiniEngine {
    struct ParsedShader {
        std::string name;
        
        std::unordered_map<ShaderType, std::string> sources;
    };

    struct ShaderParser {
    public:
        static ParsedShader parse(const std::string& mainSource) {
            ParsedShader result;
            
            // ...

            return result;
        }

    private:
        // ...
    };
}

#endif