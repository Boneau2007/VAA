
#ifndef GRAPH_DIALOG_HPP
#define GRAPH_DIALOG_HPP

#include <string>
#include "graph.hpp"
#include "fileHandler.hpp"
namespace Uebung1 {

    class GraphDialog{
        
        
        // Classattributes
        private:
            Uebung1::Graph* graph = new Graph();
            Uebung1::FileHandler* reader = new FileHandler();
            std::string fileName;
            
        public:
            // Constructors
            GraphDialog();
            GraphDialog(const std::string nodeFileName);
            GraphDialog(const std::string nodeFileName, const std::string formatFileName);
            GraphDialog(const std::string nodeFileName, const unsigned int neighborsNumber);

            // Deconstructors
            ~GraphDialog(){ delete graph; }
            
            // Inline-Elemtfunctions
            Uebung1::Graph& getGraph(){ return *graph; }
            void setGraph(Uebung1::Graph& graph){ this->graph = &graph; }

            std::string getFileName(){ return fileName; }
            void setFileName(std::string fileName){ this->fileName = fileName; }

            // Memberfunctions
            void readNodesFromFile(const std::string fileName);
            void graphgen(const std::string fileName,const unsigned int nodeCount,const unsigned int edgeCount);
            void setTopologie(const unsigned int neighborSize);
            void setTopologie();
    };
}
#endif // GRAPH_DIALOG_HPP
