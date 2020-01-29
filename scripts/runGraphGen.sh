../cmake-build-debug/graphGenApp ../config/nodeFile.txt "$1" "$2" ../graph.dot
dot -Tpng -O ./graph.dot