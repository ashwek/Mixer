#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cmath>
#include <sys/stat.h>

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
string getOutputFileName(string song1, string song2);
float dataToFloat(uint16_t num);
uint16_t floatToData(float num);
void mix(string song1, string song2);

int main(){

    char filePath[200];

    struct stat s;

    // input song 1 file path
    do{

        cout <<"Enter song 1 file path : ";
        cin.getline(filePath, 200);

        if( stat(filePath, &s) == 0 ){
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
    string song1(filePath);

    // input song 2 file path
    do{

        cout <<"Enter song 2 file path : ";
        cin.getline(filePath, 200);

        if( stat(filePath, &s) == 0 ){
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
    string song2(filePath);

    mix(song1, song2);

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


string getOutputFileName(string song1, string song2){

    /*
     * find unique file name so we dont overwrite
     * any existing files
    */

    // split file name & extension
    string song1Name, song1Extension;
    string song2Name, song2Extension;

    int lastPoint = song1.find_last_of(".");
    if( lastPoint == -1 ){
        song1Name = song1;
        song1Extension = "";
    }
    else {
        song1Name = string(song1.begin(), song1.begin() + lastPoint);
        song1Extension = string(song1.begin() + lastPoint, song1.end());
    }

    lastPoint = song2.find_last_of(".");
    if( lastPoint == -1 ){
        song2Name = song2;
        song2Extension = "";
    }
    else {
        song2Name = string(song2.begin(), song2.begin() + lastPoint);
        song2Extension = string(song2.begin() + lastPoint, song2.end());
    }

    string fileName = song1Name + " + " + song2Name + song1Extension;

    // append counter to file name until we get a unique file name
    int counter = 0;
    struct stat s;
    while( stat(fileName.c_str(), &s) == 0 ){
        counter++;
        fileName = song1Name + " + " + song2Name + " - " + to_string(counter) + song1Extension;
    }

    return fileName;

}


void mix(string song1, string song2){

    ifstream left(song1, ios::binary);
    ifstream right(song2, ios::binary);

    // read wav header
    WavHeader leftHeader, rightHeader;
    left.read((char *) &leftHeader, sizeof(leftHeader));
    right.read((char *) &rightHeader, sizeof(rightHeader));

    if( leftHeader.noChannels != 2 && leftHeader.bitsPerSample != 16){
        cout <<"Cannot use song 1";
        return;
    }

    if( rightHeader.noChannels != 2 && rightHeader.bitsPerSample != 16){
        cout <<"Cannot use song 2";
        return;
    }

    cout <<"\nMixing...\r";
    cout.flush();

    clock_t start, end;
    start = clock();

    // get unique output file name so we overwrite any file
    string outputFileName = getOutputFileName(getFileBaseName(song1), getFileBaseName(song2));

    // create output file header
    WavHeader mixHeader(0, 2, max(leftHeader.sampleRate, rightHeader.sampleRate),
                16, 0);

    // write output file header
    ofstream outFile(outputFileName, ios::binary);
    outFile.write((char*)&mixHeader, sizeof(mixHeader));

    uint32_t dataSize = 0;
    uint16_t channel1, channel2, temp;

    while( ! left.eof() || ! right.eof() ){

        if( ! left.eof() ){
            // read data from both channels
            left.read((char *) &channel1, sizeof(channel1));
            left.read((char *) &temp, sizeof(temp));

            // if data on both channels is different, get average
            if( channel1 != temp ){
                channel1 = floatToData((dataToFloat(channel1) + dataToFloat(temp)) / 2);
            }
        }

        if( ! right.eof() ){
            // read data from both channels
            right.read((char *) &channel2, sizeof(channel2));
            right.read((char *) &temp, sizeof(temp));

            // if data on both channels is different, get average
            if( channel2 != temp ){
                channel2 = floatToData((dataToFloat(channel2) + dataToFloat(temp)) / 2);
            }
        }

        // if one of the songs end before the other,
        // play the same song on both channels
        if( left.eof() ) channel1 = channel2;
        if( right.eof() ) channel2 = channel1;

        outFile.write((char*)&channel1, sizeof(channel1));
        outFile.write((char*)&channel2, sizeof(channel2));

        // count the number of bytes written
        dataSize += sizeof(channel1);
        dataSize += sizeof(channel2);

    }

    // write new data size to wav header
    outFile.seekp(40, ios_base::beg);
    outFile.write((char*)&dataSize, sizeof(dataSize));

    // write new file size to wav header
    dataSize += 16;
    outFile.seekp(4, ios_base::beg);
    outFile.write((char*)&dataSize, sizeof(dataSize));

    left.close();
    right.close();
    outFile.close();

    end = clock();

    cout <<"Completed in " <<((float)(end - start)) / CLOCKS_PER_SEC <<" seconds\n";
    cout <<"File saved as \"" <<outputFileName <<"\"\n\n";

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
