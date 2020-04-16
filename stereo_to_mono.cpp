#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sys/stat.h>
#include <cmath>


using namespace std;


class WavHeader{
public:
    // RIFF Descriptor
    uint8_t  RIFF[4];
    uint32_t fileSize;
    uint8_t WAVE[4];

    // fmt sub-chunk
    uint8_t fmt[4];
    uint32_t fmtSize;
    uint16_t audioFormat;
    uint16_t noChannels;
    uint32_t sampleRate;
    uint32_t bytesPerSec;
    uint16_t blockAlign;
    uint16_t bitsPerSample;

    // data sub-chunk
    uint8_t data[4];
    uint32_t dataSize;

    WavHeader(uint32_t fileSize, uint16_t noChannels, uint32_t sampleRate,
            uint16_t bitsPerSample, uint32_t dataSize){

        // RIFF descriptor
        RIFF[0] = 'R';RIFF[1] = 'I';RIFF[2] = 'F';RIFF[3] = 'F';
        this->fileSize = fileSize;
        WAVE[0] = 'W';WAVE[1] = 'A';WAVE[2] = 'V';WAVE[3] = 'E';

        // fmt sub-chunk
        fmt[0] = 'f';fmt[1] = 'm';fmt[2] = 't';fmt[3] = 32;
        fmtSize = 16;
        audioFormat = 1;
        this->noChannels = noChannels;
        this->sampleRate = sampleRate;
        this->bitsPerSample = bitsPerSample;
        blockAlign = (bitsPerSample * noChannels) / 8;
        bytesPerSec = (sampleRate * bitsPerSample * noChannels) / 8;

        // data sub-chunk
        data[0] = 'd';data[1] = 'a';data[2] = 't';data[3] = 'a';
        this->dataSize = dataSize;

    }

    WavHeader(){}

};


string getFileBaseName(string filePath);
string getOutputFileName(string fileName);
void combine(string filePath);
float dataToFloat(uint16_t num);
uint16_t floatToData(float num);


int main(){

    char fp[200];

    struct stat s;
    do{

        cout <<"Enter file path : ";
        cin.getline(fp, 200);

        if( stat(fp, &s) == 0 ){
            if( s.st_mode & S_IFDIR ){
                cout <<"\n *** Input path is a directory, input file path *** \n\n";
                continue;
            }
        }
        else {
            cout <<"\n *** No Such File Exists *** \n\n";
            continue;
        }

        break;

    }while( 1 );

    string filePath(fp);
    combine(filePath);

    cout << endl;
    return 0;

}


string getFileBaseName(string filePath){

    /*
     * given an absolute / relative path,
     * return only the file name
    */

    int lastPos = filePath.find_last_of("/") + 1;
    filePath.erase(filePath.begin(), filePath.begin() + lastPos);
    return filePath;

}


string getOutputFileName(string fileName){

    /*
     * find unique file name so we dont overwrite
     * any existing files
    */

    // split file name & extension
    string name, extension;

    int lastPoint = fileName.find_last_of(".");
    if( lastPoint == -1 ){
        name = fileName + " - Mono";
        extension = "";
    }
    else {
        name = string(fileName.begin(), fileName.begin() + lastPoint) + " - Mono";
        extension = string(fileName.begin() + lastPoint, fileName.end());
    }

    fileName = name + extension;

    // append counter to file name until we get a unique file name
    int counter = 0;
    struct stat s;
    while( stat(fileName.c_str(), &s) == 0 ){
        counter++;
        fileName = name + " - " + to_string(counter) + extension;
    }

    return fileName;

}


void combine(string filePath){

    ifstream inFile(filePath);

    // read input file wav header
    WavHeader header;
    inFile.read((char *) &header, sizeof(header));

    if( header.noChannels != 2 ){
        cout <<"This file is not stereo, it has " <<header.noChannels
        <<" channels\nPlease provide a stereo file";
        return;
    }

    cout <<"\nCombining channels...\r";
    cout.flush();

    clock_t start, end;
    start = clock();

    // get unique output file name
    string outputFileName = getOutputFileName(getFileBaseName(filePath));

    // change header data
    header.noChannels = 1;
    header.blockAlign = (header.bitsPerSample * header.noChannels) / 8;
    header.bytesPerSec = (header.sampleRate * header.bitsPerSample * header.noChannels) / 8;
    header.fileSize = header.dataSize = 0;

    // write wav header
    ofstream outFile(outputFileName, ios::binary);
    outFile.write((char*) &header, sizeof(header));

    uint16_t channel1, channel2;
    uint32_t dataSize = 0;
    while( ! inFile.eof() ){

        // read contents of both channel
        inFile.read((char*) &channel1, sizeof(channel1));
        inFile.read((char*) &channel2, sizeof(channel2));

        // if data on both channel is different, find average
        if( channel1 != channel2 ){
            channel1 = floatToData((dataToFloat(channel1) + dataToFloat(channel2)) / 2);
        }

        // write data of single channel to output file
        outFile.write((char*)&channel1, sizeof(channel1));

        // count the number of bytes written to file
        dataSize += sizeof(channel1);

    }

    // write new data size to wav header
    outFile.seekp(40, ios_base::beg);
    outFile.write((char*) &dataSize, sizeof(dataSize));

    // write new file size to wav header
    dataSize += 44;
    outFile.seekp(4, ios_base::beg);
    outFile.write((char*) &dataSize, sizeof(dataSize));

    inFile.close();
    outFile.close();

    end = clock();

    cout <<"Completed in " <<((float)(end - start)) / CLOCKS_PER_SEC <<" seconds\n";
    cout <<"File saved as : " <<outputFileName <<"\n\n";

}


float dataToFloat(uint16_t num){

    /*
     * converting channel data from 16-bit integer data to
     * fractional binary fixed-point numbers
     *
     * The most significant (16th) bit is used as the
     * sign bit. If sign bit is 1, it is a negative number
     * we need to find its 2's complement.
     *
     * fractional fixed-point numbers are obtained from
     * integer fixed-point numbers by dividing them by 2 ^ (N - 1)
     * here N = 16 (bitsPerSample)
    */

    uint16_t signBit = 32768;
    int multiplier = 1;

    // check if sign bit is 1
    if( signBit & num ){
        // 1's complement
        num ^= 65535;
        // +1 to get 2's complement
        num++;
        // setting multiplier to -1 to change final answer to negative
        multiplier = -1;
    }

    return multiplier * ((float)num) / pow(2, 15);

}


uint16_t floatToData(float num){

    /*
     * converting fractional fixed-point number
     * to 16-bit integer
     *
     * This is just the reverse process of dataToFloat(..)
    */

    // if num is negative, set signBit to 1
    uint16_t signBit = (num < 0) ? 32768 : 0;

    num = abs(num);
    num *= pow(2, 15);

    uint16_t ans = num;

    // if it is a negative number, get 2's complement
    if( signBit ){
        // 1's complement (invert all bits)
        ans ^= 65535;
        // +1 to get 2's complement
        ans++;
    }

    return ans;

}
