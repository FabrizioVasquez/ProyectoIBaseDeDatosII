#include<iostream>
#include<fstream>
using namespace std;


struct Data
{
    size_t id,ciclo;
    string nombre;
    long next;

    Data(){}

};

template <typename XDT>
struct Index
{
    string typeIndex;
    string filename;


    Index(string typeIndex, string filename)
    {
        this->typeIndex = typeIndex;
        this->filename = filename;
    }
    ~Index(){}
};

template <typename XDT>
class SequentialFile : public Index<XDT>
{
public:

    fstream fileSequential;

    SequentialFile(string typeIndex, string filename):Index<XDT>(typeIndex,filename){
        fileSequential.open(filename, std::ios::in | std::ios::out);

    }

    void readAll()
    {

        fileSequential.close();
    }

    // read a specific record by index
    void read(Data record)
    {

    }

    // insert a record by index
    void insert(Data record)
    {

    }

    // search a record by key
    void search()
    {

    }
};

template <typename XDT>
class Hash : public Index<XDT> {
public:

    fstream fileHash;

    Hash(string typeIndex, string filename) : Index<XDT>(typeIndex, filename) {
        fileHash.open(filename, std::ios::in | std::ios::out);

    }
    void readAll()
    {

        fileHash.close();
    }

    // read a specific record by index
    void read(Data record)
    {

    }

    // insert a record by index
    void insert(Data record)
    {

    }

    // search a record by key
    void search()
    {

    }
};

template< typename TYPE_RECORD>
class Directory {
    Directory(string filename, string index);
    void readAll();
    void read(TYPE_RECORD record);
    void insert(TYPE_RECORD record);
    void search();
};


template<>
class Directory <Data>
{
private:
    string filename;
    // fstream fileStorage;
    string indexName;
    Index<Data> *indexSequentialFile;
    Index<Data> *indexHash;

public:


    Directory(string filename, string indexName)
    {
        this->filename = filename;
        this->indexName = indexName;

        if(indexName == "SEQUENTIALFILE")
        {
            indexSequentialFile = new SequentialFile<Data>("SequentialFile",filename);
            cout<<"HOLA ESTAMOS EN EL INDICE SEQUENCIAL"<<endl;
        }
        else if(indexName == "HASH")
        {
            indexHash = new Hash<Data>("HASH",filename);
            cout<<"HOLA ESTAMOS EN EL INDICE HASH"<<endl;
        }
    }

    void readAll()
    {

    }

    // read a specific record by index
    void read(Data record)
    {

    }

    // write a record by index
    void insert(Data record)
    {

    }

    // search a record by key
    void search()
    {

    }
    ~Directory(){
    }
};

int main(){

    Directory<Data> directorio("data.csv","SEQUENTIALFILE");


    cout<<"HELLO WORLD"<<endl;
    return 0;
}