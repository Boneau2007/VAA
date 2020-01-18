./createNodeFile.sh $1
bin/graphGenApp nodeFile.txt graph.dot $2
dot -Tpng -O graph.dot