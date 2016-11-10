// SAMPLE TEST_APP
// illustrating usage of the jpeg encoder
#include "jpegenc.h"

char  *in_fname,*out_fname;

void usage()
{
    printf("Usage:\n");
    printf("jpegenc.exe -q <quality> -w <width> -h <height> -f <pixel_format> -o <outfile> <infile>\n");
    printf("Supported formats:\n");
    printf("YUV400\t0YUV444\t1YUV420\t2YUV422\t3YUV422\t4PPM(RGB24)\t5\n");
    printf("-q  -> quality (1 .. 100)\n");

    return;
}

// sample wrapper function for testing jpeg_encoder
int main(int argc,char *argv[])
{
    FILE *fp;

    unsigned char val,byte,format,optimize,subsample;

    int i,j;
    int cols,rows,depth,quality;

    unsigned int bufsize = 0,bytes_written = 0;

    unsigned char *raw_input,*jpeg_output;

    unsigned int *temp;
    // structs
    static jcinfo_t jc_info;

    ///////////////////////////////////////////////////////////////////////////////////////////////

    //used to get buffer sizes
    jc_init_encode(&jc_info);

    out_fname = in_fname = NULL;
    cols = rows = 0;
    quality = 75;//default value

    format = 5;//default format is ppm
    optimize = JC_DEFAULT_HUFFMAN_TABLES;
    subsample = JC_DISABLE_SUBSAMPLING;

    i = 1;
    while(--argc)
    {
	if(strcmp(argv[i],"-w")==0)
	{
	    i++;
	    argc--;
	    cols = atoi(argv[i]);
	}
	else if(strcmp(argv[i],"-h")==0)
	{
	    i++;
	    argc--;
	    rows = atoi(argv[i]);
	}
	else if(strcmp(argv[i],"-optimize")==0)
	{
	    optimize = JC_OPTIMIZE_HUFFMAN_TABLES;
	}
	else if(strcmp(argv[i],"-s")==0)
	{
	    subsample = JC_ENABLE_SUBSAMPLING;
	}
	else if(strcmp(argv[i],"-o")==0)
	{
	    i++;
	    argc--;
	    out_fname = argv[i];
	}
	else if(strcmp(argv[i],"-q")==0)
	{
	    i++;
	    argc--;
	    quality = atoi(argv[i]);
	}
	else if(strcmp(argv[i],"-f")==0)
	{
	    i++;
	    argc--;
	    format = atoi(argv[i]);
	    if(format==JC_INPUT_RGB24)				
		cols = rows = 0;
	}
	else
	{
	    in_fname = argv[i];
	}

	i++;
    }

    if(in_fname==NULL)
    {
	printf("Enter input file name.\n");
	usage();
	exit(-1);
    }

    if((quality<0)||(quality>100))
    {
	printf("Invalid quality factor.Should be a value between (1...100).\n");
	usage();
	exit(-1);
    }

    if((format<0)||(format>5))
    {
	printf("Invalid input pixel format.\n");
	usage();
	exit(-1);
    }

    jc_info.jc_input_format = format;

    //This part of the code is used to extract name of the jpeg image from the complete file path

    if((fp = fopen(in_fname, "rb"))==NULL)
    {
	printf("Error accessing %s\n",in_fname);
	exit(-1);
    }

    if(jc_info.jc_input_format==JC_INPUT_RGB24)
    {
	byte = fgetc(fp);
	if(byte != 'P')
	{
	    printf("Fatal error: Image starts not with P.\n");
	    exit(0);
	}

	byte = fgetc(fp);
	if(byte != '6')
	{
	    printf("Fatal error: Not a valid ppm image.\n");
	    exit(0);
	}

	byte = fgetc(fp);//newline

	// get rid of those damn comments!
	byte = fgetc(fp);
	if(byte == '#')
	{
	    do
	    {
		byte = fgetc(fp);
	    } while(byte != '\n');//tolerate crap
	}
	else
	{
	    ungetc((int)byte, fp);//wow! no crap!
	}

	fscanf(fp,"%d %d\n%d",&cols,&rows,&depth);
	byte = fgetc(fp);//the damn '\n' after depth!
    }

    if((cols==0)&&(rows==0))
    {
	printf("Enter width and height for YUV format.\n");
	usage();
	exit(-1);
    }

    if(out_fname==NULL)
    {
	j = 0;
	for(i=0;in_fname[i]!='\0';i++)
	{
	    if(in_fname[i] ==92)
		j = i + 1;
	}

	i = 0;
	while(in_fname[j] != '.')
	{
	    in_fname[i] = in_fname[j];
	    i++;
	    j++;
	}
	in_fname[i++] = '.';		
	in_fname[i++] = 'j';
	in_fname[i++] = 'p';
	in_fname[i++] = 'g';
	in_fname[i] = '\0';

	out_fname = in_fname;
    }

    ////////////////////////////////////////////////////////////////////////////
    //																		  //
    //		JPEG Encode starts here											  //
    //																		  //
    ////////////////////////////////////////////////////////////////////////////

    //buffer mallocs
    temp = (unsigned int *)malloc(jc_info.l1_buf_size);
    if(temp==NULL)
    {
	printf("Error allocating memory for l1_buf.\n");
	exit(-1);
    }
    jc_info.l1_buf = (void *)temp;
#if DEBUG_INFO
    printf("jc_info.l1_buf = %p\n",temp);
#endif
    temp = (unsigned int *)malloc(jc_info.l3_buf_size);
    if(temp==NULL)
    {
	printf("Error allocating memory for l3_buf.\n");
	exit(-1);
    }
    jc_info.l3_buf = (void *)temp;
#if DEBUG_INFO
    printf("jc_info.l3_buf = %p\n",temp);
#endif

    jc_info.cols = cols;
    jc_info.rows = rows;

    bufsize = 3*jc_info.cols*jc_info.rows;
    raw_input = (unsigned char *)malloc(sizeof(unsigned char)*bufsize);
    if(raw_input == NULL)
    {
	printf("Error allocating memory for raw_input.\n");
	exit(-1);
    }
#if DEBUG_INFO
    printf("raw_input = %p\n",raw_input);
#endif

    fread(raw_input,sizeof(unsigned char),bufsize,fp);
    fclose(fp);

    jpeg_output = (unsigned char *)malloc(sizeof(unsigned char)*bufsize*5);
    if(jpeg_output == NULL)
    {
	printf("Error allocating memory for jpeg_output.\n");
	exit(-1);
    }

    jc_info.jc_input_buf = raw_input;
    jc_info.jc_input_buf_size = bufsize;
    jc_info.jc_output_buf = jpeg_output;
    jc_info.jc_output_buf_size = 0;

    jc_set_defaults(&jc_info);

    jc_info.jc_quality_factor = quality;

    jc_info.jc_optimize = optimize;

    jc_info.jc_subsample = subsample;

    //////////////////////////////////////////////////////////////////////////
    //		jc_write_headers() fills all the necessary fields in the		//
    //		jpeg output buffer												//
    //////////////////////////////////////////////////////////////////////////
    val = jc_write_headers(&jc_info);
    //val should be JC_WRITE_HEADERS_SUCCESS (0x64) for successful operation

    printf("val after write headers: %d\n",val);
    fflush(NULL);

    if(val==JC_WRITE_HEADERS_SUCCESS)
    {
	//////////////////////////////////////////////////////////////////////////
	//	jc_encode() encodes the uncompressed data and gives jpeg output		//
	//////////////////////////////////////////////////////////////////////////
	val = jc_encode(&jc_info);
	//if image is encoded successfully, val = JC_ENCODE_SUCCESS
	printf("val after jpeg encode: %d\n",val);

	free(raw_input);

	free(jc_info.l1_buf);

	free(jc_info.l3_buf);

	bufsize = jc_info.jc_output_buf_size;

	///////////////////////////////////////////////////////////
	//			jpeg encode ends here.						 //
	///////////////////////////////////////////////////////////
	if((fp = fopen(out_fname,"wb"))==NULL)
	{
	    printf("Error creating %s\n",out_fname);
	    exit(-1);
	}
	bytes_written = fwrite(jpeg_output,sizeof(unsigned char),bufsize,fp);
	printf("bytes_written = %d\n",bytes_written);
	fclose(fp);

	free(jpeg_output);
    }
    else
    {
	free(raw_input);
	free(jpeg_output);
	free(jc_info.l1_buf);
	free(jc_info.l3_buf);
    }

    printf("\n===============================================================\n\n");

    return 0;
}
