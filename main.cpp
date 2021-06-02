#include<iostream>
#include<fstream>
#include <utility>
#include<vector>
#include<sstream>
#include<ctime>
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
            ofstream response(filename, std::ios::out | std::ios::trunc);
            for(auto&i:this->records){
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
    std::vector<XDT> insert( XDT record)
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
                //cout<<"Position: "<<positionVec<<endl;
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
        return this->records;
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
        auto pos = positionBefore(start_key);
        if(pos != -1) {
            for (int i = pos; i < records.size(); i++) {
                if (records[i].id > start_key && records[i].id < end_key) {
                    recordsByRange.push_back(records[i]);
                }
            }
        }
        else {
            for (auto &item:records) {
                if (item.id > start_key && item.id < end_key) {
                    recordsByRange.push_back(item);
                }
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
            indexSequentialFile->writeRecords(this->indexSequentialFile->insert(record),"dataInsert.csv");
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
    //directorio.printAll();
    cout<<endl;
    //directorio.insert(data1);
    cout<<endl;
    //directorio.printAll();
    //directorio.writeAll("data3.csv");
    //cout<<endl;
    //directorio.searchByRange(400,850);// FUNCIONA CON LOS DATOS ENTREGADOS
    //directorio.search(345); // Funciona y retorna y file con los valores entregados.
    //directorio.printAll();
    string path;
    size_t option;
    cout<<"***********************************************************************"<<endl;
    cout<<"***********************************************************************"<<endl;
    cout<<"***********************************************************************"<<endl;
    cout<<"******HOLA BIENVENIDO A SU SISTEMA GESTOR DE BASE DE DATOS MINIMO******"<<endl;
    cout<<"***********************************************************************"<<endl;
    cout<<"***********************************************************************"<<endl;
    cout<<"***********************************************************************\n\n"<<endl;
    cout<<"Escriba la ruta absoluta de su archivo a procesar: ";cin>>path;
    if(path.size() < 20)
    {
        cout<<"Que tecnica de organización de archivo desea utilizar ?"<<endl;
        cout<<"1. Secuential File"<<endl;
        cout<<"2. Extendible Hashing"<<endl;
        cout<<"3. No se que es eso (by default)"<<endl;
        cout<<"Digite el numero de la opción: ";cin>>option;
        if(option > 1 ) {
            Directory<Data, int> directorioHash(path, "HASH");
            int optionHash;
            while(true) {
                cout << "\nTenemos las siguientes operaciones disponibles: " << endl;
                cout << "1.Inserción de un registro" << endl;
                cout << "2.Busqueda individial de registro por key" << endl;
                cout << "3.Busqueda por rangos dado dos keys" << endl;
                cout << "-1. Salir" << endl;
                cout << "Digite el numero de la opción: ";
                cin >> optionHash;
                if(optionHash == -1) break;
                if(optionHash == 1)
                {

                }
                if(optionHash == 2)
                {

                }
                if(optionHash == 3)
                {

                }
            }
        }
        else
        {
            Directory<Data,int> directorioSequential(path,"SEQUENTIALFILE");
            int optionSecuencial;
            while(true) {
                cout << "\nTenemos las siguientes operaciones disponibles: " << endl;
                cout << "1.Inserción de un registro" << endl;
                cout << "2.Busqueda individial de registro por key" << endl;
                cout << "3.Busqueda por rangos dado dos keys" << endl;
                cout << "-1. Salir" << endl;
                cout << "Digite el numero de la opción: ";
                cin >> optionSecuencial;
                if(optionSecuencial == -1) break;
                if(optionSecuencial == 1)
                {
                    string ID,NOMBRE,CICLO;
                    cout<<"Por favor escriba el ID del registro: "<<endl;cin>>ID;
                    cout<<"Por favor escriba el NOMBRE del registro: "<<endl;cin>>NOMBRE;
                    cout<<"Por favor escriba el ciclo del registro: "<<endl;cin>>CICLO;
                    Data dato(stoi(ID),NOMBRE,stoi(CICLO));
                    directorioSequential.insert(dato);
                    cout<<"Gracias! Su archivo se encuentra insertado en el archivo dataInsert.csv";
                }
                if(optionSecuencial == 2)
                {
                    int key;
                    cout<<"Por favor escriba la key (ID) del registro a buscar: ";cin>>key;

                    cout<<"Gracias! Su archivo se encuentra insertado en el archivo dataSearch.csv";
                }
                if(optionSecuencial == 3)
                {
                    int start_key,end_key;
                    cout<<"Por favor escriba el rango de la key (ID) del conjunto de registros a buscar: ";cin>>start_key;
                    cout<<"Por favor escriba el rango de la key (ID) del conjunto de registros a buscar: ";cin>>end_key;
                    directorioSequential.searchByRange(start_key,end_key);
                    cout<<"Gracias! Su archivo se encuentra insertado en el archivo dataSearchByRange.csv";
                }
            }
        }
    }

    return 0;
}