#include "Map.hpp"

Map::Map(const std::string& filename) {
    std::ifstream   infile(filename);
    std::string     vertex;

    if (!infile.is_open())
        throw std::runtime_error(ERR_OPEN(filename));

    while (!infile.eof()){
        infile >> vertex;
        if (infile.eof()) break;

        QVector3D res = Map::_parseVertex(vertex);
        if (!this->contains(res))
            this->append(res);
    }
    infile.close();
}

QVector3D Map::_parseVertex(const std::string& vertex) {
    std::stringstream stream(vertex);
    float x,y,z;
    char sep1,sep2;
    char par1,par2;

    if (vertex.find_first_of(')') != vertex.length() - 1
        || vertex.find_last_of('(') != 0)
        throw runtime_error(ERR_INPUT_FILE ERR_SYNTAX);
    stream >> par1 >> x >> sep1 >> y >> sep2 >> z >> par2;
    if (!stream || sep1 != ',' || sep2 != ',' || par1 != '(' || par2 != ')')
        throw runtime_error(ERR_INPUT_FILE ERR_SYNTAX);

    std::string rest;
    stream >> rest;
    if (stream)
        throw runtime_error(ERR_INPUT_FILE ERR_SYNTAX);
    return {x, y, z};
}

std::ostream &operator<<(ostream &o, const Map &rhs){
    o << "{";

    for (const auto &item: rhs){
        if (item != rhs.first())
            o << ", ";
        o << "{" << item.x() << "," << item.y() << "," << item.z() << "}";
    }
    o << "}";
    return o;
}
