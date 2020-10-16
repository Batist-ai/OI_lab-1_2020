#include "rgbhsv.h"

#define CLAMP(_v, _min, _max)\
	(_v < _min ? _min: _v > _max ? _max: _v)
#define HUE_ANGLE 60.0f

#define geta(_type, _x, _n)\
    _type _x[_n];\
    for (int _i = 0; _i < _n; _i++)\
        cin >> _x[_i];


// rgb to hsv and hsv to rgb convertions
namespace rgbhsv
{
	void rgb_to_hsv(int r, int g, int b, float *r_h, float *r_s, float *r_v) {
		float rp, gp, bp, cmax, cmin, delta, l;
		int cmaxwhich, cminwhich;

		rp = ((float)r) / 255;
		gp = ((float)g) / 255;
		bp = ((float)b) / 255;

		//printf("rgb=%d,%d,%d rgbprime=%f,%f,%f\n", r, g, b, rp, gp, bp);

		cmax = rp;
		cmaxwhich = 0; /* faster comparison afterwards */
		if (gp > cmax) { cmax = gp; cmaxwhich = 1; }
		if (bp > cmax) { cmax = bp; cmaxwhich = 2; }
		cmin = rp;
		cminwhich = 0;
		if (gp < cmin) { cmin = gp; cminwhich = 1; }
		if (bp < cmin) { cmin = bp; cminwhich = 2; }

		//debug ("cmin=%f,cmax=%f", cmin, cmax);
		delta = cmax - cmin;

		/* HUE */
		if (delta == 0) {
			*r_h = 0;
		}
		else {
			switch (cmaxwhich) {
			case 0: /* cmax == rp */
				*r_h = HUE_ANGLE * (fmod((gp - bp) / delta, 6));
				break;

			case 1: /* cmax == gp */
				*r_h = HUE_ANGLE * (((bp - rp) / delta) + 2);
				break;

			case 2: /* cmax == bp */
				*r_h = HUE_ANGLE * (((rp - gp) / delta) + 4);
				break;
			}
			if (*r_h < 0)
				*r_h += 360;
		}

		/* LIGHTNESS/VALUE */
		//l = (cmax + cmin) / 2;
		*r_v = cmax;

		/* SATURATION */
		/*if (delta == 0) {
		  *r_s = 0;
		} else {
		  *r_s = delta / (1 - fabs (1 - (2 * (l - 1))));
		}*/
		if (cmax == 0) {
			*r_s = 0;
		}
		else {
			*r_s = delta / cmax;
		}
		//printf("rgb=%d,%d,%d ---> hsv=%f,%f,%f\n", r, g, b, *r_h, *r_s, *r_v);
	}


	void hsv_to_rgb(float h, float s, float v, int *r_r, int *r_g, int *r_b) {
		if (h > 360)
			h -= 360;
		if (h < 0)
			h += 360;
		h = CLAMP(h, 0, 360);
		s = CLAMP(s, 0, 1);
		v = CLAMP(v, 0, 1);
		float c = v * s;
		float x = c * (1 - fabsf(fmod((h / HUE_ANGLE), 2) - 1));
		float m = v - c;
		float rp, gp, bp;
		int a = h / 60;

		//debug ("h=%f, a=%d", h, a);

		switch (a) {
		case 0:
			rp = c;
			gp = x;
			bp = 0;
			break;

		case 1:
			rp = x;
			gp = c;
			bp = 0;
			break;

		case 2:
			rp = 0;
			gp = c;
			bp = x;
			break;

		case 3:
			rp = 0;
			gp = x;
			bp = c;
			break;

		case 4:
			rp = x;
			gp = 0;
			bp = c;
			break;

		default: // case 5:
			rp = c;
			gp = 0;
			bp = x;
			break;
		}

		*r_r = (rp + m) * 255;
		*r_g = (gp + m) * 255;
		*r_b = (bp + m) * 255;

		//debug ("hsv=%f,%f,%f, ---> rgb=%d,%d,%d", h, s, v, *r_r, *r_g, *r_b);
	}
}