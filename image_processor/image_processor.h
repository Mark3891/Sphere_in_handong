class ImageProcessor{
    public:
        //ratio will only accept the value as multiples of 30
        ImageProcessor(int height, int width, int tpad, int sratio, int eratio, int rotation, int* rows, int row_size){
            this->height = height;
            this->width = width;
            this->tpad = tpad;
            this->sratio = (sratio/30)*30%360;
            this->eratio = (eratio/30)*30%360;
            this->row_size = row_size;
            this->rows = rows;
            this->calc_row_size = row_size-tpad;
            this->rotation = (rotation/30)*30%360;
            if(row_size > tpad)
                calc_rows = new int[row_size-tpad];
            calc_row();
            
            int pixel_sum=0;
            for(int i = 0; i < row_size; i++){
                pixel_sum+=rows[i];
            }
            processed_image = new unsigned char[pixel_sum];
            this->processed_image_size = pixel_sum;
        }
        int calc_row();
        int mask(unsigned char *image);
        int rotate();
        int set_rotation(int rotation){
            this->rotation = (rotation/30)*30%360;
            return 0;
        }
        int set_tpad(int tpad){
            this->tpad = tpad;
            calc_row();
            return 0;
        }
        unsigned char * get_processed_image(){
            return processed_image;
        }
        unsigned char *image;
        ~ImageProcessor(){
            delete[] calc_rows;
            delete[] rows;
            delete[] partial_buf;
            delete[] processed_image;
        }

    private:
        int width;
        int height;
        int tpad;
        int sratio;
        int eratio;
        int rotation;
        int row_size;
        int calc_row_size;
        int partial_buf_size;
        int processed_image_size;
        int *calc_rows;
        int *rows;
        unsigned char *partial_buf;
        unsigned char *processed_image;
};

