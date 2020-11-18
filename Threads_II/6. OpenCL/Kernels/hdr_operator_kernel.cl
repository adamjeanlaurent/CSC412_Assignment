__kernel void hdr_operator(
						 __global int* cinput,
                         __global int* uinput,
                         __global int* oinput,
						 __global int* output,
                         const unsigned int count)
{
    const unsigned char LOW_BOUND_R = 30;
    const unsigned char HIGH_BOUND_R = 200;
    const unsigned char LOW_BOUND_G = 30;
    const unsigned char HIGH_BOUND_G = 200;
    const unsigned char LOW_BOUND_B = 30;
    const unsigned char HIGH_BOUND_B = 180;
    
    const int LOW_VAL = 1, MED_VAL = 2, HIGH_VAL = 3;
    
    int i = get_global_id(0);
    int scount = (int) count;
    if(i < scount) {

#if 0
//        output[i] = input[i] & 0x00FF00FF;
        output[i] = input[i] & 0x00FF0000;
#else
        // extract r, g, b components from cinput[i]
        unsigned char cred = (cinput[i] & 0x000000FF);
        unsigned char cgreen = (cinput[i] & 0x0000FF00)>>8;
        unsigned char cblue = (cinput[i] & 0x00FF0000)>>16;
        
        // extract r, g, b components from uinput[i]
        unsigned char ured = (uinput[i] & 0x000000FF);
        unsigned char ugreen = (uinput[i] & 0x0000FF00)>>8;
        unsigned char ublue = (uinput[i] & 0x00FF0000)>>16;
        
        // extract r, g, b components from oinput[i]
        unsigned char ored = (oinput[i] & 0x000000FF);
        unsigned char ogreen = (oinput[i] & 0x0000FF00)>>8;
        unsigned char oblue = (oinput[i] & 0x00FF0000)>>16;
 
        /**
        // compute gray = max(r, g, b)
        unsigned char gray;
        if(red>green && red>blue)
            gray=red;
        else if(green>red && green>blue)
            gray=green;
        else
            gray=blue;
    
        // produce output[i] = g g g 255
        output[i]=gray<<16 | gray<<8 | gray | 0xFF000000;
//        output[i]= ((input[i] >> 24) & 0x000000FF << 24) | 0x000000FF;
//        output[i] = input[i];
*/

        unsigned char redRegion, blueRegion, greenRegion;
        
        if(cred <LOW_BOUND_R)
            redRegion = LOW_VAL;
        else if (cred<=HIGH_BOUND_R)
            redRegion = MED_VAL;
        else
            redRegion = HIGH_VAL;
        //
        if(cgreen <LOW_BOUND_G)
            greenRegion = LOW_VAL;
        else if (cgreen<=HIGH_BOUND_G)
            greenRegion = MED_VAL;
        else
            greenRegion = HIGH_VAL;
        //
        if(cblue <LOW_BOUND_B)
            blueRegion = LOW_VAL;
        else if (cblue<=HIGH_BOUND_B)
            blueRegion = MED_VAL;
        else
            blueRegion = HIGH_VAL;
        
        int exposureRegion = 100*redRegion + 10*greenRegion + blueRegion;
        switch(exposureRegion)
        {
            case 111:
                output[i] = oblue<<16 | ogreen<<8 | ored | 0xFF000000;
                break;
                
            case 112:
                output[i] = oblue<<16 | ogreen<<8 | ored | 0xFF000000;
                break;
                
            case 113:
                output[i] = cblue<<16 | cgreen<<8 | cred | 0xFF000000;
                break;
                
            case 121:
                output[i] = oblue<<16 | ogreen<<8 | ored | 0xFF000000;
                break;
                
            case 122:
                output[i] = oblue<<16 | ogreen<<8 | ored | 0xFF000000;
                break;
                
            case 123:
                output[i] = cblue<<16 | cgreen<<8 | cred | 0xFF000000;
                break;
                
            case 131:
                output[i] = cblue<<16 | cgreen<<8 | cred | 0xFF000000;
                break;
                
            case 132:
                output[i] = cblue<<16 | cgreen<<8 | cred | 0xFF000000;
                break;
                
            case 133:
                output[i] = cblue<<16 | cgreen<<8 | cred | 0xFF000000;
                break;
                
            case 211:
                output[i] = oblue<<16 | ogreen<<8 | ored | 0xFF000000;
                break;
                
            case 212:
                output[i] = oblue<<16 | ogreen<<8 | ored | 0xFF000000;
                break;
                
            case 213:
                output[i] = cblue<<16 | cgreen<<8 | cred | 0xFF000000;
                break;

            case 221:
                output[i] = oblue<<16 | ogreen<<8 | ored | 0xFF000000;
                break;
                
            case 222:
                output[i] = cblue<<16 | cgreen<<8 | cred | 0xFF000000;
                break;
                
            case 223:
                output[i] = ublue<<16 | ugreen<<8 | ured | 0xFF000000;
                break;
                
            case 231:
                output[i] = cblue<<16 | cgreen<<8 | cred | 0xFF000000;
                break;
                
            case 232:
                output[i] = ublue<<16 | ugreen<<8 | ured | 0xFF000000;
                break;
                
            case 233:
                output[i] = ublue<<16 | ugreen<<8 | ured | 0xFF000000;
                break;

            case 311:
                output[i] = cblue<<16 | cgreen<<8 | cred | 0xFF000000;
                break;
                
            case 312:
                output[i] = cblue<<16 | cgreen<<8 | cred | 0xFF000000;
                break;
                
            case 313:
                output[i] = cblue<<16 | cgreen<<8 | cred | 0xFF000000;
                break;
                
            case 321:
                output[i] = cblue<<16 | cgreen<<8 | cred | 0xFF000000;
                break;
                
            case 322:
                output[i] = ublue<<16 | ugreen<<8 | ured | 0xFF000000;
                break;
                
            case 323:
                output[i] = ublue<<16 | ugreen<<8 | ured | 0xFF000000;
                break;
                
            case 331:
                output[i] = cblue<<16 | cgreen<<8 | cred | 0xFF000000;
                break;
                
            case 332:
                output[i] = ublue<<16 | ugreen<<8 | ured | 0xFF000000;
                break;
                
            case 333:
                output[i] = ublue<<16 | ugreen<<8 | ured | 0xFF000000;
                break;
                
        }
//        unsigned char red;
//        if(cred>=LOW_BOUND_R && cred<=HIGH_BOUND_R)
//            red=cred;
//        else if(cred<LOW_BOUND_R)
//            red=ored;
//        else
//            red=ured;
//        
//        unsigned char green;
//        if(cgreen>=LOW_BOUND_G && cgreen<=HIGH_BOUND_G)
//            green=cgreen;
//        else if(cgreen<LOW_BOUND_G)
//            green=ogreen;
//        else
//            green=ugreen;
//        
//        unsigned char blue;
//        if(cblue>=LOW_BOUND_B && cblue<=HIGH_BOUND_B)
//            blue=cblue;
//        else if(cblue<LOW_BOUND_B)
//            blue=oblue;
//        else
//            blue=ublue;
        
//        output[i] = blue<<16 | green<<8 | red | 0xFF000000;
#endif
    }
}

