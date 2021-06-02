#include<iostream>
#include<fstream>
#include <utility>
#include<vector>
#include<sstream>
#include<ctime>
using namespace std;

struct Bucket{
    int Depth = 0;
    string bucketName = "";
    int next_1 = 0;
    int next_2 = 0;
    int next_overflow = 0;
    Bucket(string currentName, int Depth, int id){
        this->Depth = Depth;
        this->bucketName = currentName + to_string(id);
    };
    Bucket(){};
};

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
      
    string filename;

    Hash() {
	filename = "HashingIndex.dat";
        fstream fileHash;
        this->filename = filename;
        fileHash.open(filename, std::ios::out | std::ios::app);
        fileHash.close();
        fileHash.open(filename, std::ios::in | std::ios::out | ios::binary);
        fileHash.seekg(0, ios::end);  
        if (fileHash.tellg() == 0){
            //Create initial Buckets - 0
            Bucket bucket_0("", 1, 0);
            fileHash.write((char*) &bucket_0.Depth, sizeof(bucket_0.Depth));
            int size1 = bucket_0.bucketName.size();
            fileHash.write((char*) &size1, sizeof(size1));
            fileHash << bucket_0.bucketName;
            fileHash.write((char*) &bucket_0.next_1, sizeof(bucket_0.Depth));
            fileHash.write((char*) &bucket_0.next_2, sizeof(bucket_0.Depth));
            fileHash.write((char*) &bucket_0.next_overflow, sizeof(bucket_0.Depth));
            fstream bucket_temp;
            bucket_temp.open(bucket_0.bucketName+".dat", std::ios::out | std::ios::app);
            bucket_temp.close();

            //Create initial Buckeets - 1
            Bucket bucket_1("", 1, 1);
            fileHash.write((char*) &bucket_1.Depth, sizeof(bucket_1.Depth));
            int size2 = bucket_1.bucketName.size();
            fileHash.write((char*) &size2, sizeof(size2));
            fileHash << bucket_1.bucketName;
            fileHash.write((char*) &bucket_1.next_1, sizeof(bucket_1.Depth));
            fileHash.write((char*) &bucket_1.next_2, sizeof(bucket_1.Depth));
            fileHash.write((char*) &bucket_1.next_overflow, sizeof(bucket_1.Depth));
            fstream bucket_temp1;
            bucket_temp1.open(bucket_1.bucketName+".dat", std::ios::out | std::ios::app);
            bucket_temp1.close();
        }
        fileHash.close();
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

    string hash(int i){
        string answer;
        if(i == 1){
            answer = "0101";
        }else{
            answer = "1010";
        }
        return answer;
    }

    void readAllBuckets()
    {
        ifstream fileHash;
        fileHash.open(filename, ios::binary);
        Bucket temp;
        int aux;
        while(!fileHash.eof()){
            fileHash.read((char *)&temp.Depth, sizeof(temp.Depth));
            fileHash.read((char *)&aux, sizeof(aux));
		    temp.bucketName.resize(aux);
		    fileHash.read(&temp.bucketName[0], aux);
            fileHash.read((char *)&temp.next_1, sizeof(temp.next_1));
            fileHash.read((char *)&temp.next_2, sizeof(temp.next_2));
            fileHash.read((char *)&temp.next_overflow, sizeof(temp.next_overflow));
            cout << temp.Depth << " " << temp.bucketName << " " << temp.next_1 << " " << temp.next_2 << " " << temp.next_overflow << "\n";
        }
        fileHash.close();
    }

    void readAll(){
        vector<Data> answer;
        ifstream fileHash;
        fileHash.open(filename, ios::in | ios::binary);
        Bucket temp;
        int aux;
        int counter1 = 0;
        int counter2 = 0;
        while(!fileHash.eof()){
            counter1++;
            fileHash.read((char *)&temp.Depth, sizeof(temp.Depth));
            fileHash.read((char *)&aux, sizeof(aux));
		    temp.bucketName.resize(aux);
		    fileHash.read(&temp.bucketName[0], aux);
            fileHash.read((char *)&temp.next_1, sizeof(temp.next_1));
            fileHash.read((char *)&temp.next_2, sizeof(temp.next_2));
            fileHash.read((char *)&temp.next_overflow, sizeof(temp.next_overflow));
            counter2 = 0;
            if(temp.next_1 == 0 && temp.next_2 == 0){
                fstream bucket;
                Data example;
                int size1;
                bucket.open(temp.bucketName+".dat", std::ios::in | std::ios::binary);
                while(!bucket.eof()){
                counter2++;
                bucket.read((char*) &example.id, sizeof(example.id));
                
                bucket.read((char*) &example.ciclo, sizeof(example.ciclo));
                
                bucket.read((char*) &size1, sizeof(size1));
                example.nombre.resize(size1);
                bucket.read((char*) &example.nombre[0], size1);
                bucket.read((char*) &example.next, sizeof(example.next));
                if (example.ciclo != 0)
                answer.push_back(example);
                }
                answer.pop_back();
                bucket.close();
                
            }
            
        }
        for(int i = 0; i < counter2-1; i++)
            answer.pop_back();
        fileHash.close();
        return answer;
    }

    // read a specific record by index
    void read(int index)
    {
        int depth = 1;
        string key = "";
        key+= hash(index)[depth];
        string next_key = "" + hash(index)[depth+1];
        ifstream fileHash;
        Bucket temp;
        int aux;
        fileHash.open(filename, ios::binary);
        while(!fileHash.eof()){
            fileHash.read((char *)&temp.Depth, sizeof(temp.Depth));
            fileHash.read((char *)&aux, sizeof(aux));
		    temp.bucketName.resize(aux);
		    fileHash.read(&temp.bucketName[0], aux);
            fileHash.read((char *)&temp.next_1, sizeof(temp.next_1));
            fileHash.read((char *)&temp.next_2, sizeof(temp.next_2));
            fileHash.read((char *)&temp.next_overflow, sizeof(temp.next_overflow));
            
            
            if(key == temp.bucketName){
                if(next_key == "0" and &temp.next_1 != 0){
                depth++;
                key = key + next_key;
                next_key = "" + hash(index)[depth-1];
                //fileHash.seekg(temp.next_1, ios::beg);
                }
                else if(next_key == "1" and &temp.next_1 !=0){
                depth++;
                key = key + next_key;
                next_key = hash(index)[depth+1];
                //fileHash.seekg(temp.next_2, ios::beg);
                }else{
                break;
                }
            }   
        }
        fileHash.close();
        
        fstream bucket;
        bucket.open(temp.bucketName+".dat", std::ios::in | std::ios::binary);
        Data record;
        int size1;
        while(!bucket.eof()){
            bucket.read((char*) &record.id, sizeof(record.id));
            bucket.read((char*) &record.ciclo, sizeof(record.ciclo));
            bucket.read((char*) &size1, sizeof(size1));
            record.nombre.resize(size1);
            bucket.read((char*) &record.nombre[0], size1);
            bucket.write((char*) &record.next, sizeof(record.next));
            
            if(record.id == index){
                cout << "id: " << record.id << "\nciclo: " << record.ciclo << "\nnombre :" << record.nombre << endl;
                return;
            };

        }
            bucket.close();

    }

    // insert a record by index
    void insert(Data record)
    {   
        //bool de necesidad de mergear
        bool SPLIT_FLAG = false;


        int depth = 1;
        string key = "";
        key+= hash(record.id)[depth];
        string next_key = "" + hash(record.id)[depth+1];
        cout << "For " << record.nombre << ": " << key << "\n";

        ifstream fileHash;
        fileHash.open(filename, ios::binary);
        Bucket temp;
        int aux;
        int prev_bucket_position = 0;
        
        //Encontramos en qué bucket debe ir
        while(!fileHash.eof()){
            fileHash.read((char *)&temp.Depth, sizeof(temp.Depth));
            fileHash.read((char *)&aux, sizeof(aux));
		    temp.bucketName.resize(aux);
		    fileHash.read(&temp.bucketName[0], aux);
            fileHash.read((char *)&temp.next_1, sizeof(temp.next_1));
            fileHash.read((char *)&temp.next_2, sizeof(temp.next_2));
            fileHash.read((char *)&temp.next_overflow, sizeof(temp.next_overflow));
            
            if(key == temp.bucketName){
                if(next_key == "0" and &temp.next_1 != 0){
                depth++;
                key = key + next_key;
                cout << "\n" << "HOLA" <<key << "\n\n";
                next_key = "" + hash(record.id)[depth-1];
                //fileHash.seekg(temp.next_1, ios::beg);
                }
                else if(next_key == "1" and &temp.next_1 !=0){
                depth++;
                key = key + next_key;
                next_key = hash(record.id)[depth+1];
                //fileHash.seekg(temp.next_2, ios::beg);
                }else{
                break;
                }
                

            }else{prev_bucket_position += 1;}
            
        }
        fileHash.close();
        //Escribimos en el bucket indicado
        fstream bucket;
        bucket.open(temp.bucketName+".dat", std::ios::out | std::ios::app);
        bucket.write((char*) &record.id, sizeof(record.id));
        bucket.write((char*) &record.ciclo, sizeof(record.ciclo));
        int size1 = record.nombre.size();
        bucket.write((char*) &size1, sizeof(size1));
        bucket << record.nombre;
        bucket.write((char*) &record.next, sizeof(record.next));
        bucket.close();

        
        //Leemos el Bucket indicado para verificar el límite
        ifstream bucket2;
        Data aux2;
        int counter = 0;
        bucket2.open(temp.bucketName+".dat", std::ios::in | ios::binary);
        int size2;
        vector<Data> bucket_data;
        while(!bucket2.eof()){
                bucket2.read((char*) &aux2.id, sizeof(aux2.id));
                cout << aux2.id << " ";
                bucket2.read((char*) &aux2.ciclo, sizeof(aux2.ciclo));
                cout << aux2.ciclo << " ";
                bucket2.read((char *)&size2, sizeof(size2));
		        aux2.nombre.resize(size2);
		        bucket2.read(&aux2.nombre[0], size2);
                cout << aux2.nombre << " ";
                bucket2.read((char*) &aux2.next, sizeof(aux2.next));
                counter++;
                cout << counter << "\n";
                bucket_data.push_back(aux2);
                if(counter-1 > Bucket_size) SPLIT_FLAG = true;
        }
        bucket2.close();



        //CUANDO SE EXCEDE LA CAPACIDAD DE UN BUCKET

        if(SPLIT_FLAG == true){
            //SI AÚN NO ALCANZAMOS EL GLOBAL DEPTH
            if(temp.Depth <= global_Depth)
            {
                //CREATE NEXT TWO BUCKETS
                string deleteName = temp.bucketName + ".dat";
                remove(&deleteName[0]);
                fstream nextone;
                fstream nextzero;
                nextzero.open(temp.bucketName+"0.dat", std::ios::out | std::ios::app);
                nextzero.close();
                nextone.open(temp.bucketName+"1.dat", std::ios::out | std::ios::app);
                nextone.close();
            }

            //ESCRIBIR EN EL INDICE LOS DOS NUEVOS BUCKETS
            ofstream fileHash2;
            fileHash2.open(filename, std::ios::app | ios::binary);
            Bucket bucket_0(temp.bucketName, temp.Depth+1, 0);
            fileHash2.write((char*) &bucket_0.Depth, sizeof(bucket_0.Depth));
            int size1 = bucket_0.bucketName.size();
            fileHash2.write((char*) &size1, sizeof(size1));
            fileHash2 << bucket_0.bucketName;
            fileHash2.write((char*) &bucket_0.next_1, sizeof(bucket_0.Depth));
            fileHash2.write((char*) &bucket_0.next_2, sizeof(bucket_0.Depth));
            fileHash2.write((char*) &bucket_0.next_overflow, sizeof(bucket_0.Depth));

            Bucket bucket_1(temp.bucketName, temp.Depth+1, 1);
            fileHash2.write((char*) &bucket_1.Depth, sizeof(bucket_1.Depth));
            int size2 = bucket_1.bucketName.size();
            fileHash2.write((char*) &size2, sizeof(size2));
            fileHash2 << bucket_1.bucketName;
            fileHash2.write((char*) &bucket_1.next_1, sizeof(bucket_1.Depth));
            fileHash2.write((char*) &bucket_1.next_2, sizeof(bucket_1.Depth));
            fileHash2.write((char*) &bucket_1.next_overflow, sizeof(bucket_1.Depth));
            fileHash2.close();

            //PONER EL FLAG DE NEXT_1 Y NEXT_2 A LOS BUCKETS
            
            fstream fileHash3;
            fileHash3.open(filename, ios::in | ios::out | ios::binary);

            fstream falsecopy;
            falsecopy.open("temp.dat", ios::out | ios::binary);

            Bucket temp;
            int aux;

            bool flagito2 = false;
            if(prev_bucket_position == 0){
                fileHash3.read((char *)&temp.Depth, sizeof(temp.Depth));
            fileHash3.read((char *)&aux, sizeof(aux));
		    temp.bucketName.resize(aux);
		    fileHash3.read(&temp.bucketName[0], aux);
            fileHash3.read((char *)&temp.next_1, sizeof(temp.next_1));
            fileHash3.read((char *)&temp.next_2, sizeof(temp.next_2));
            fileHash3.read((char *)&temp.next_overflow, sizeof(temp.next_overflow));
            flagito2 = true;
            }


            while(prev_bucket_position > 0){
            
            fileHash3.read((char *)&temp.Depth, sizeof(temp.Depth));
            fileHash3.read((char *)&aux, sizeof(aux));
		    temp.bucketName.resize(aux);
		    fileHash3.read(&temp.bucketName[0], aux);
            fileHash3.read((char *)&temp.next_1, sizeof(temp.next_1));
            fileHash3.read((char *)&temp.next_2, sizeof(temp.next_2));
            fileHash3.read((char *)&temp.next_overflow, sizeof(temp.next_overflow));
            
            falsecopy.write((char *)&temp.Depth, sizeof(temp.Depth));
            falsecopy.write((char *)&aux, sizeof(aux));
            falsecopy << temp.bucketName;
            falsecopy.write((char *)&temp.next_1, sizeof(temp.next_1));
            falsecopy.write((char *)&temp.next_2, sizeof(temp.next_2));
            falsecopy.write((char *)&temp.next_overflow, sizeof(temp.next_overflow));

            prev_bucket_position--;

            }
            

            temp.next_1 = 1;
            temp.next_2 = 1;
            
            falsecopy.write((char*) &temp.Depth, sizeof(temp.Depth));
            int size3 = temp.bucketName.size();
            falsecopy.write((char*) &size3, sizeof(size3));
            falsecopy << temp.bucketName;
            falsecopy.write((char*) &temp.next_1, sizeof(temp.Depth));
            falsecopy.write((char*) &temp.next_2, sizeof(temp.Depth));
            falsecopy.write((char*) &temp.next_overflow, sizeof(temp.Depth));
            bool flagito = false;

            while(!fileHash3.eof()){
            fileHash3.read((char *)&temp.Depth, sizeof(temp.Depth));
            fileHash3.read((char *)&aux, sizeof(aux));
		    temp.bucketName.resize(aux);
		    fileHash3.read(&temp.bucketName[0], aux);
            fileHash3.read((char *)&temp.next_1, sizeof(temp.next_1));
            fileHash3.read((char *)&temp.next_2, sizeof(temp.next_2));
            fileHash3.read((char *)&temp.next_overflow, sizeof(temp.next_overflow));
            if(flagito or flagito2){
            falsecopy.write((char *)&temp.Depth, sizeof(temp.Depth));
            falsecopy.write((char *)&aux, sizeof(aux));
            falsecopy << temp.bucketName;
            falsecopy.write((char *)&temp.next_1, sizeof(temp.next_1));
            falsecopy.write((char *)&temp.next_2, sizeof(temp.next_2));
            falsecopy.write((char *)&temp.next_overflow, sizeof(temp.next_overflow));
            }
            flagito = true;
            }


            fileHash3.close();
            falsecopy.close();
            remove("HashIndex.dat");
            rename("temp.dat", "HashIndex.dat");
        }


    }




    // search a record by key
    vector<Data> search(int low, int high){
        vector<Data> answer;
        ifstream fileHash;
        fileHash.open(filename, ios::binary);
        Bucket temp;
        int aux;
        while(!fileHash.eof()){
            fileHash.read((char *)&temp.Depth, sizeof(temp.Depth));
            fileHash.read((char *)&aux, sizeof(aux));
		    temp.bucketName.resize(aux);
		    fileHash.read(&temp.bucketName[0], aux);
            fileHash.read((char *)&temp.next_1, sizeof(temp.next_1));
            fileHash.read((char *)&temp.next_2, sizeof(temp.next_2));
            fileHash.read((char *)&temp.next_overflow, sizeof(temp.next_overflow));
            cout << temp.Depth << " " << temp.bucketName << " " << temp.next_1 << " " << temp.next_2 << " " << temp.next_overflow << "\n";

            if(temp.next_1 == 0 && temp.next_2 == 0){
                fstream bucket;
                Data example;
                bucket.open(temp.bucketName+".dat", std::ios::in | std::ios::binary);
                while(!bucket.eof()){
                bucket.read((char*) &example.id, sizeof(example.id));
                bucket.read((char*) &example.ciclo, sizeof(example.ciclo));
                int size1 = example.nombre.size();
                bucket.read((char*) &size1, sizeof(size1));
                example.nombre.resize(size1);
                bucket.write((char*) &example.next, sizeof(example.next));
                bucket.close();
                if(example.id < high && example.id > low)
                answer.push_back(example);
                }
            }
        }
        fileHash.close();
        return answer;
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
                    directorioSequential.search(key);
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
