#include<iostream>
#include<fstream>
#include <utility>
#include<vector>
#include<sstream>
using namespace std;



struct Data
{
    size_t id{},ciclo{};
    char nombre[20];
    long next{};
    int nextDel{};

    Data(){}
    Data(size_t id, string nombre, size_t ciclo, long next = 0){
        this->id = id;
        for (int i = 0; i < nombre.size(); ++i) this->nombre[i] = nombre[i];
        this->ciclo = ciclo;
        this->next = next;
        this->nextDel = 0;
    }
    Data& operator =(const Data &p)
    {
        this->id = p.id;
        //for (int i = 0; i < p.nombre; ++i) this->nombre[i] = p.nombre[i];
        strncpy(this->nombre, p.nombre, sizeof(p.nombre));
        //p[sizeof(p.nombre) - 1] = '\0';
        this->ciclo = p.ciclo;
        this->next = p.next;
        return *this;
    }

    friend ostream & operator << (ostream & stream, Data & alumno) {
        stream.write((char*)&alumno, sizeof(alumno));
        return stream;
    }
    friend istream & operator >> (istream & stream, Data & alumno) {
        stream.read((char*)&alumno, sizeof(alumno));
        return stream;
    }

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

template <typename XDT, typename Key>
class SequentialFile : public Index<XDT>
{
public:

    fstream fileSequential;
    string filenameResponse,auxFile;
    vector<XDT> records;

    SequentialFile(string typeIndex, string filename):Index<XDT>(typeIndex,filename){
        this->load();
        //std::copy(begin(records),end(records), back_inserter(originalRecords));
    }

    static bool criterion(XDT data1, XDT data2){
        return (data1.id) < (data2.id);
    }

    vector<XDT> load()
    {
        fileSequential.open(this->filename, std::ios::binary | std::ios::in | std::ios::out);
        try {
            string line;
            string header;
            ifstream read_file(this->filename);
            getline(read_file,header);
            int i = 0;
            while(getline(read_file,line)) {
                string id{};
                string nombre{};
                string ciclo{};

                std::stringstream ss(line);

                std::getline(ss, id, ',');
                std::getline(ss, nombre, ',');
                std::getline(ss, ciclo);
                Data aux(stoi(id),nombre,stoi(ciclo));
                ++i;
                records.emplace_back(stoi(id),nombre,stoi(ciclo),0);
            }
            read_file.close();
        }
        catch (std::ifstream::failure &e) {
            std::cerr << "Exception opening/reading/closing file\n";
        }
        // back to the start!
        fileSequential.close();
        for (int i = 0; i < records.size(); ++i) {
            records[i].nextDel = records[i+1].id;
            if(i == records.size() - 1){
                records[i].nextDel = records[i].id;
            }
        }

        return records;
    }

    string writeRecords(std::vector<XDT> records_ ,const string &outputFilename)
    {
        try{
            ofstream response(outputFilename, std::ios::out | std::ios::trunc);
            for(auto&i:records_)
            {
                response  << i.id<<",";
                response  << i.nombre<<",";
                response  << i.ciclo;
                response << '\n';
            }
            response.close();
        }catch (std::ostream::failure &e) {
            std::cerr << "Exception opening/reading/closing file\n";
        }
        return outputFilename;
    }

    string writeAll(const string& filename)
    {
        try{
            ofstream response(filename, std::ios::out | std::ios::app);
            for(auto&i:records){
                response  << i.id<<",";
                response  << i.nombre<<",";
                response  << i.ciclo;
                response << '\n';
            }
            response.close();
        }catch (std::ostream::failure &e) {
            std::cerr << "Exception opening/reading/closing file\n";
        }
        return filename;
    }


    // insert a record on final
    void insert( XDT record)
    {

        std::sort(begin(records),end(records), criterion);

        //fileSequential.open(this->filename,ios::in| ios::out| ios::binary);
        records.push_back(record);
        std::sort(begin(records),end(records), criterion);

        int positionVec = 0;
        XDT previosData;
        XDT nextData;
        for(auto it = begin(records); it!= end(records); it++)
        {
            positionVec++;
            if(it->id == record.id){
                cout<<"Position: "<<positionVec<<endl;
                previosData = records[positionVec-2];
                nextData = records[positionVec];
            }
        }
        auto actualValueNextDel = records[positionVec-3].nextDel;
        //cout<<positionVec<<"<-"<<endl;
        //cout<<records[positionVec-2].nombre   <<"<-";
        //previosData.nextDel = record.id;
        records[positionVec-3].nextDel = record.id;
        records[positionVec-2].nextDel = nextData.id;

        //cout<<actualValueNextDel<<endl;

        /*for(int i = positionVec-2 ; i < records.size(); i++){
            records[i].next += sizeof(XDT);
        }*/

    }

    void printAll()
    {
        for(auto&i:this->records)
            cout<<i.id<<" "<<i.nombre<<" "<<i.ciclo<<" "<<i.next<<" "<<i.nextDel<<endl;
    }


    // return record by key
    std::vector<XDT> search(int key)
    {
        vector<XDT> resutls;
        if(records.size() > 0)
        {
            int start = 0;
            int end = records.size() - 1;
            int mid = (start + end) / 2;
            while (start<=end)
            {
                if (records[mid].id < key)
                {
                    start=mid+1;
                }
                else if (records[mid].id == key)
                {
                    resutls.push_back(records[mid]);
                    return resutls;
                }
                else
                {
                    end = mid - 1;
                }
                mid = (start+end)/2;
            }
        }
        fileSequential.close();
    }

    // search by range and key
    std::vector<XDT> searchByRange(int start_key, int end_key)
    {
        std::vector<XDT> recordsByRange;
        for(auto&item:records)
        {
            if(item.id > start_key && item.id < end_key)
            {
                recordsByRange.push_back(item);
            }
        }
        return recordsByRange;
    }

    // return position by key
    int positionBefore(int key)
    {
        fstream newFile(this->filename,std::ios::in | std::ios::binary );
        //this->fileSequential.open(this->filename,std::ios::in | std::ios::out);
        if(newFile.is_open())
        {
            string line,n_line;
            int i = -1;
            getline(newFile,n_line);
            while(getline(newFile,line))
            {
                string id;
                string nombre;
                string ciclo;

                std::stringstream ss(line);

                std::getline(ss, id, ',');
                std::getline(ss, nombre, ',');
                std::getline(ss, ciclo);
                XDT record(stoi(id),nombre,stoi(ciclo));
                i++;
                if((record.id) == key)
                    return i;
            }
        }
        return -1;
    }

    bool delete_(int pos)
    {
        if (pos < 0 || pos > records.size()-1)
            return false;

        int alumnoSize = (int)sizeof(XDT);
        int deletePosicion = pos * alumnoSize;
        XDT temp1, temp2;

        readRecord(this->fileSequential, temp1, deletePosicion);
        if (temp1.next == 0)
            return false;
        if (pos == 0 || temp1.next == -1) {
            temp1.next = 0;
            writeRecord(this->fileSequential,temp1, 0, false);
            return true;
        }

        // Encontrar Registro Previo
        int posicionPrevia;
        string Filename;
        int alumnoPrevio = Search(stol(temp1.Dni)-1,true);

        readRecord(this->fileSequential, temp2, alumnoPrevio*alumnoSize);
        if (temp2.next > 0) {
            Filename = this->fileSequential;
            posicionPrevia = alumnoPrevio*alumnoSize;
        } else {
            posicionPrevia = -1*(temp2.next+alumnoSize);
            readRecord(auxFile, temp2, -1*(temp2.next+alumnoSize));
            while (temp2.next < 0) {
                posicionPrevia = -1*(temp2.next+alumnoSize);
                readRecord(auxFile, temp2, posicionPrevia);
            }
            Filename = auxFile;
        }

        // Sobreescribir Alumnos
        if (temp2.next == deletePosicion) {
            temp2.next = temp1.next;
            temp1.next = 0;
            writeRecord(this->fileSequential, temp1, deletePosicion, false);
            writeRecord(Filename, temp2, posicionPrevia, false);
            return true;
        }
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

template< typename TYPE_RECORD, typename Key>
class Directory {
    Directory(string filename, string index);
    void readAll();
    void read(TYPE_RECORD record);
    void insert(TYPE_RECORD record);
    void search();
};


template<typename Key>
class Directory <Data,Key>
{
private:
    string filename;
    // fstream fileStorage;
    string indexName;
    SequentialFile<Data,Key> *indexSequentialFile;
    Hash<Data> *indexHash;

public:

    Directory(string filename, string indexName)
    {
        this->filename = filename;
        this->indexName = indexName;

        if(indexName == "SEQUENTIALFILE")
        {
            indexSequentialFile = new SequentialFile<Data,Key>("SequentialFile",filename);
            cout<<"HOLA ESTAMOS EN EL INDICE SEQUENCIAL"<<endl;
        }
        else if(indexName == "HASH")
        {
            indexHash = new Hash<Data>("HASH",filename);

        }
    }

    vector<Data> readAll()
    {
        if(indexName == "SEQUENTIALFILE")
        {
            return indexSequentialFile->load();
        }
    }

    string writeAll(const string& filename_)
    {
        if(indexName == "SEQUENTIALFILE")
        {
            return indexSequentialFile->writeAll(filename_);
        }
    }

    // read a specific record by index

    // write a record by index
    void insert(Data record)
    {
        if(indexName == "SEQUENTIALFILE")
        {
            indexSequentialFile->insert(record);
            this->indexSequentialFile->writeRecords(this->indexSequentialFile->records,"dataInsert.csv");
        }
    }

    void printAll()
    {
        if(indexName == "SEQUENTIALFILE")
        {
            indexSequentialFile->printAll();
        }
    }

    // search a record by key
    void search(int key)
    {
        if(indexName == "SEQUENTIALFILE")
        {
            //cout<<indexSequentialFile->search(key);
            this->indexSequentialFile->writeRecords(indexSequentialFile->search(key),"dataSearch.csv");
        }
    }

    void searchByRange(int start_key, int end_key)
    {
        if(indexName == "SEQUENTIALFILE")
        {
            this->indexSequentialFile->writeRecords(indexSequentialFile->searchByRange(start_key,end_key),"dataSearchByRange.csv");
        }
    }


    ~Directory(){
    }
};

int main(){

    Directory<Data,int> directorio("data.csv","SEQUENTIALFILE");
    Data data1(829,"yo",7);
    //directorio.readAll();
    directorio.printAll();
    cout<<endl;
    directorio.insert(data1);
    cout<<endl;
    //directorio.printAll();
    //directorio.writeAll("data3.csv");
    //cout<<endl;
    //directorio.searchByRange(400,850);// FUNCIONA CON LOS DATOS ENTREGADOS
    //directorio.search(345); // Funciona y retorna y file con los valores entregados.
    //directorio.printAll();


    return 0;
}