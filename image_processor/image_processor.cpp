#include <cstring>
#include <iostream>

using namespace std;

#define GET_RPOS(H, W, X, Y) (((Y)*(W)*3)+(X))
#define GET_GPOS(H, W, X, Y) (((Y)*(W)*3)+(X)+1)
#define GET_BPOS(H, W, X, Y) (((Y)*(W)*3)+(X)+2)

/*
class ImageProcessor{
    public:
        //ratio will only accept the value as multiples of 30
        ImageProcessor(int height, int tpad, int sratio, int eratio, int* rows, int row_size){
            this->height = height;
            this->tpad = tpad;
            this->sratio = (sratio/30)*30;
            this->eratio = (eratio/30)*30;
            this->row_size = row_size;
            this->rows = rows;
            if(row_size > tpad)
                calc_rows = new int[row_size-tpad];
            calc_row();
        }
        int calc_row();
        int mask(unsigned char *image);
        int rotate();
        int toArray();
        int clear();
        unsigned char *image;

    private:
        int height;
        int tpad;
        int sratio;
        int eratio;
        int row_size;
        int* calc_rows;
        int* rows;
};
*/

#include "image_processor.h"

/**
 * calculate row for spherical display using end-ratio, start-ratio
 *
 * returns
 * 0 : success
 * 1 : null error
 * 2 : ratio error
 */
int ImageProcessor::calc_row(){
    int *p_rows = rows+tpad;
    int dratio = eratio-sratio;
    int sum = 0;
    if(!rows) return 1;
    if(!calc_rows) return 1;
    if(dratio < 30) return 2;
    for(int i = 0; i < calc_row_size; i++){
        calc_rows[i] = *(p_rows+i)*(dratio/360.0);
        sum+=calc_rows[i];
    }
    partial_buf_size = sum*3;
    partial_buf = new unsigned char[partial_buf_size];
    for(int i = 0; i < calc_row_size; i++){
        cout << calc_rows[i] << ',';
    }
    cout << endl;
    return 0;
}

int ImageProcessor::mask(unsigned char *image){
    if(!rows || !calc_rows || !partial_buf) return 1;
    int image_size = width * height * 3;
    int mask_height = height/calc_row_size;
    int idx = 0;
    for(int i = 0; i < calc_row_size; i++){
        int mask_width = width/calc_rows[i];
        /*
        cout << width <<" / " << calc_rows[i] << " = ";
        cout << mask_width << "," << mask_height << endl; 
        */

        for(int j = 0; j < calc_rows[i]; j++){
            long long rsum = 0;
            long long gsum = 0;
            long long bsum = 0;
            int mask_size = mask_height * mask_width;
            for(int xmask = 0; xmask < mask_width; xmask++){
                for(int ymask = 0; ymask < mask_height; ymask++){
                   // cout << (int)image[GET_RPOS(height, width, j*mask_width+xmask, i*mask_height+ymask)] << ' ';
                    rsum += image[GET_RPOS(height, width, j*mask_width+xmask, i*mask_height+ymask)];
                    gsum += image[GET_GPOS(height, width, j*mask_width+xmask, i*mask_height+ymask)];
                    bsum += image[GET_BPOS(height, width, j*mask_width+xmask, i*mask_height+ymask)];
                }
            }
            partial_buf[idx] = bsum/mask_size;
            partial_buf[idx+1] = gsum/mask_size;
            partial_buf[idx+2] = rsum/mask_size;
            idx+=3;
        }
    }
    cout << endl;
    /*
    for(int i = 0; i < partial_buf_size; i++){
        cout << (int)partial_buf[i] << " ";
    }
    cout << endl;
    */
    return 0;
}

int ImageProcessor::rotate(){
    unsigned char * p_partial_buf = partial_buf;
    long long idx = 0;
    int h = 0;
    if(!processed_image) return 1;

    memset(processed_image, 0, processed_image_size);
    while(idx < processed_image_size){
        cout << h << endl;
        if(h >= tpad+calc_row_size){
            break;
        }
        if(h < tpad){
            idx+=rows[h];
            h++;
            continue;
        }
        int lpad = (sratio/360.0)*rows[h];
        memcpy(processed_image+(idx+lpad), p_partial_buf, min(rows[h]-lpad, calc_rows[h-tpad]));
        cout << rows[h]-lpad << "," << calc_rows[h-tpad] << endl;
        if(rows[h]-lpad < calc_rows[h-tpad]){
            memcpy(processed_image+idx, p_partial_buf+(rows[h]-lpad+1), calc_rows[h-tpad]-(rows[h]-lpad));
        }
        cout << "2" << endl;
        p_partial_buf+=calc_rows[h-tpad];
        idx+=calc_rows[h-tpad];
        h++;
    }
    return 0;
}


