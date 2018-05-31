/* $LastChangedDate: 2018-01-28 13:56:03 +0100 (Sun, 28 Jan 2018) $ */
/*
 Image.h : Image class headers

 (c) 2007-2018 Michele Martone

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#ifndef FIM_IMAGE_H
#define FIM_IMAGE_H

#include "FbiStuff.h"
#include "fim.h"
#if FIM_WANT_PIC_CMTS
#include <fstream>
#include <istream>
#include <ios>
#include <string>
#include <sstream>
#endif /* FIM_WANT_PIC_CMTS */
#include <memory>

namespace fim
{
enum fim_cvd_t /* color vision deficiency  */ {
       	FIM_CVD_NO=0, /* no deficiency */
	FIM_CVD_PROTANOPIA=1, /* a red/green color deficiency */
	FIM_CVD_DEUTERANOPIA=2, /* a red/green color deficiency */
       	FIM_CVD_TRITANOPIA=3 /* a blue/yellow color deficiency */
};

enum fim_tii_t /* test image index  */ {
       	FIM_TII_NUL=0,
       	FIM_TII_16M=1
};

/* pixel intensity float */
#if FIM_USE_CXX11
	using fim_pif_t = float;
#else /* FIM_USE_CXX11 */
typedef float fim_pif_t;
#endif /* FIM_USE_CXX11 */

class Image FIM_FINAL 
#ifdef FIM_NAMESPACES
:public Namespace
#else /* FIM_NAMESPACES */
#endif /* FIM_NAMESPACES */
{
	public:

	explicit Image(const fim_char_t *fname, FILE *fd=FIM_NULL, fim_page_t page = 0);
	~Image(void);

	int n_pages(void)const;
	bool is_multipage(void)const;
	bool is_mirrored(void)const;
	bool is_flipped(void)const;
	bool have_nextpage(int j=1)const;
	bool have_prevpage(int j=1)const;
	bool have_page(int page)const;

	private:
	Image& operator= (const Image& i){return *this;/* a nilpotent assignment */}
	fim_scale_t            scale_;	/* viewport variables */
	fim_scale_t            ascale_;
	fim_scale_t            newscale_;
	fim_scale_t            angle_;
	fim_scale_t            newangle_;// 0-360 degrees
	fim_page_t		 page_;

	public:
	enum { FIM_ROT_L=3,FIM_ROT_R=1,FIM_ROT_U=2 };
	const struct ida_image *get_ida_image(void)const{ return img_; }
	private:
        mutable struct ida_image *img_     ;     /* local (eventually) copy images */
#if FIM_WANT_MIPMAPS
	fim_mipmap_t mm_;
#endif /* FIM_WANT_MIPMAPS */
	struct ida_image *fimg_    ;     /* master image */

	bool load(const fim_char_t *fname, FILE *fd, int want_page);
	public:
	void should_redraw(enum fim_redraw_t sr = FIM_REDRAW_NECESSARY) { redraw_ = sr; }  /* for Viewport after drawing */

	private:
	fim_redraw_t redraw_;
	enum { FIM_NO_ROT=0,FIM_ROT_ROUND=4 };
	enum { FIM_ROT_L_C='L',FIM_ROT_R_C='R',FIM_ROT_U_C='U' };
	enum { FIM_I_ROT_L=0, FIM_I_ROT_R=1}; /* internal */
	fim_pgor_t              orientation_;	// orthogonal rotation

	fim_image_source_t fis_;

	fim_fn_t fname_;	/* viewport variable, too */
	size_t fs_;		/* file size */

        void free(void);
	void reset(void);

	public:
        bool is_tiny(void)const;
	void reset_view_props(void);
	void set_auto_props(fim_int autocenter, fim_int autotop);
	virtual size_t byte_size(void)const;

	bool can_reload(void)const;
	fim_err_t update_meta(bool fresh);

	fim::string getInfo(void)const;
	Image(const Image& rhs); // yes, a private constructor (was)
#if FIM_WANT_BDI
	Image(enum fim_tii_t tii=FIM_TII_NUL);
#endif	/* FIM_WANT_BDI */
	fim_err_t do_scale_rotate( fim_scale_t ns=0.0 );
	private:
	fim_err_t do_rotate( void );
	public:
	fim_err_t rotate( fim_angle_t angle_=1.0 );

	const fim_char_t* getName(void)const;
	cache_key_t getKey(void)const;

	fim_err_t reduce( fim_scale_t factor=FIM_CNS_SCALEFACTOR);
	fim_err_t magnify(fim_scale_t factor=FIM_CNS_SCALEFACTOR, fim_bool_t aes=false);
	
	fim_pgor_t getOrientation(void)const;

	fim_err_t set_scale(fim_scale_t ns);
	fim_scale_t get_scale(void)const{return scale_;};
	int get_page(void)const{return page_+1;};
	size_t get_file_size(void)const{return fs_;/* need a Browser::file_info_cache */};
	size_t get_pixelmap_byte_size(void)const;
	fim_err_t scale_multiply (fim_scale_t sm);
	fim_scale_t ascale(void)const{ return (ascale_>0.0?ascale_:1.0); }
	void shred(void);
	fim_err_t negate (void);
	fim_err_t identity (void);
	fim_err_t desaturate (void);
	bool colorblind(enum fim_cvd_t cvd, bool daltonize);
	bool check_valid(void)const;

	int width(void)const;
	fim_coo_t original_width(void)const;
	int height(void)const;
	fim_coo_t original_height(void)const;
	bool goto_page(fim_page_t j);

#if FIM_WANT_MIPMAPS
	void mm_free(void);
	void mm_make(void);
	bool has_mm(void)const;
	size_t mm_byte_size(void)const;
#endif /* FIM_WANT_MIPMAPS */
	bool cacheable(void)const;
	void desc_update(void);
	fim_bool_t need_redraw(void)const{ return (redraw_ != FIM_REDRAW_UNNECESSARY); }
	bool fetchExifToolInfo(const fim_char_t *fname);
	fim_int shall_mirror(void)const;
	fim_int check_flip(void)const;
	fim_int check_negated(void)const;
	fim_int check_desaturated(void)const;
	fim_int check_autotop(void)const;
	fim_int check_autocenter(void)const;
	void set_exif_extra(fim_int shouldrotate, fim_int shouldmirror, fim_int shouldflip);
	void get_irs(char *irs)const;
}; /* class Image */
#if FIM_USE_CXX11
#if FIM_IMG_NAKED_PTRS
	using ImagePtr = Image*;
	using ImageCPtr = const Image*;
#else /* FIM_IMG_NAKED_PTRS */
	using ImagePtr = std::shared_ptr<Image>;
	using ImageCPtr = std::shared_ptr<const Image>;
#endif /* FIM_IMG_NAKED_PTRS */
#else /* FIM_USE_CXX11 */
	typedef Image* ImagePtr;
	typedef const Image* ImageCPtr;
#endif /* FIM_USE_CXX11 */
} /* namespace fim */
#if FIM_WANT_PIC_LVDN
	class VNamespace FIM_FINAL: public Namespace
	{
	       	public:
		const Var & setVariable(const fim_var_id& varname,const Var& value){return Namespace::setVariable(varname,value);}
		size_t byte_size(void)const
		{
		       	size_t bs=0;
			for( variables_t::const_iterator it = variables_.begin();it != variables_.end(); ++it )
				bs += it->first.size() + sizeof(it->first),
				bs += it->second.size() + sizeof(it->second);
			return bs;
	       	};
       	};
#endif /* FIM_WANT_PIC_LVDN */

#if FIM_WANT_PIC_CMTS

class ImgDscs FIM_FINAL: public std::map<fim_fn_t,fim_ds_t>
{
	public:
#if FIM_WANT_PIC_LVDN
#if FIM_USE_CXX11
	using vd_t = std::map<fim_fn_t,VNamespace> ;
#else /* FIM_USE_CXX11 */
	typedef std::map<fim_fn_t,VNamespace> vd_t;
#endif /* FIM_USE_CXX11 */
	vd_t vd_;
#endif /* FIM_WANT_PIC_LVDN */
	template<class T> struct ImgDscsCmp:public std::binary_function<typename T::value_type, typename T::mapped_type, bool>
	{
		public:
		bool operator() (const typename T::value_type &vo, const typename T::mapped_type mo) const
		{
			return vo.second == mo;
		}
	};
	ImgDscs::iterator li_;
	ImgDscs::iterator fo(const key_type& sk, const ImgDscs::iterator & li)
	{
		return std::find_if(li,end(),std::bind2nd(ImgDscsCmp<ImgDscs>(),sk));
	}
	ImgDscs::const_iterator fo(const key_type& sk, const ImgDscs::const_iterator & li)const
	{
#if FIM_USE_CXX11
		return std::find_if(li,cend(),std::bind2nd(ImgDscsCmp<ImgDscs>(),sk));
#else /* FIM_USE_CXX11 */
		return std::find_if(li,end() ,std::bind2nd(ImgDscsCmp<ImgDscs>(),sk));
#endif /* FIM_USE_CXX11 */
	}
public:
	ImgDscs::const_iterator fo(const key_type& sk)const
	{
#if FIM_USE_CXX11
		return std::find_if(cbegin(),cend(),std::bind2nd(ImgDscsCmp<ImgDscs>(),sk));
#else /* FIM_USE_CXX11 */
		return std::find_if( begin(), end(),std::bind2nd(ImgDscsCmp<ImgDscs>(),sk));
#endif /* FIM_USE_CXX11 */
	}
private:
	ImgDscs::iterator fi(const key_type& sk)
	{
		ImgDscs::iterator li;

		if(li_ == end())
			li_ = begin();
		li = li_ = fo(sk,li_);
		if(li_ == end())
			li_ = begin();
		else
			++li_;
		return li;
	}
public:
	void reset(void)
	{
		li_=begin();
	}
	ImgDscs(void)
	{
		reset();
	}
	std::string expand(const VNamespace & ns, const fim_fn_t ds)const
	{
#if FIM_WANT_DESC_VEXP
							fim_fn_t ss; // substituted string
							const fim_char_t ec = '@';
							std::string::size_type bci = 0, eci = 0;
							while ( ( bci = ds.find(ec,eci) ) != std::string::npos )
							{
								ss+=ds.substr(eci,bci-eci);
								if(ds[eci=++bci]=='#')
								{
									eci=ds.size();
									break;
								}
								if(!fim_is_id_char(ds[eci=bci]))
								{
									ss+='@';
									continue;
								}
								while(eci < ds.size() && fim_is_id_char(ds[++eci]))
									;
								if(ns.isSetVar(ds.substr(bci,eci-bci)))
									ss+=ns.getStringVariable(ds.substr(bci,eci-bci));
								else
									ss+='@',
									ss+=ds.substr(bci,eci-bci); // might issue warning instead
							}
							ss+=ds.substr(eci);
							return ss;
#else /* FIM_WANT_DESC_VEXP */
							return ds;
#endif /* FIM_WANT_DESC_VEXP */
	}
	void fetch(const fim_fn_t& dfn, const fim_char_t sc)
	{
		/* dfn: descriptions file name */
		/* sc: separator char */
		std::ifstream mfs (dfn.c_str(),std::ios::app);
		std::string ln;
#if FIM_WANT_PIC_CCMT
		std::string cps,cas,din; // contextual prepend/append/dirname string
#endif /* FIM_WANT_PIC_CCMT */
#if FIM_WANT_PIC_RCMT
		std::string ld; // last description
#endif /* FIM_WANT_PIC_RCMT */
#if FIM_WANT_PIC_LVDN
		VNamespace ns;
#endif /* FIM_WANT_PIC_LVDN */
		bool imgdscs_want_basename = true; /* FIXME: shall be more clear/flexible with this */
		// fim_fms_t dt;
		// dt = - getmilliseconds();

		while( std::getline(mfs,ln))
		{
			std::stringstream  ls(ln);
			key_type fn;
			const fim_char_t nl = '\n';
			fim_fn_t ds;

#if FIM_WANT_PIC_LVDN
			if( ls.peek() == FIM_SYM_PIC_CMT_CHAR )
			{
				if( std::getline(ls,fn,nl) )
				{
					size_t vn = fn.find("!fim:",1); /* vn will point to first variable id char */

					if( vn != std::string::npos && fn[vn+=5] )
					{
						size_t es = fn.find_first_of("=",vn); /* point to the first equal sign char */

						if( es != std::string::npos )
						{
							std::string varname = fn.substr(vn,es-vn);
							++es;
#if FIM_WANT_PIC_CCMT
							/* FIXME: rationalize this code */
							if( varname == "^" )
							{
								std::string varval = fn.substr(es);
								if( fn[es] )
									cps = varval;
								else
									cps = "";
							}
							else
							if( varname == "+" )
							{
								std::string varval = fn.substr(es);
								if( fn[es] )
									cas = varval;
								else
									cas = "";
							}
							else
							if( varname == "!" )
							{
								ns = VNamespace();//reset
							}
							else
							if( varname == "/" )
							{
								std::string varval = fn.substr(es);
								if( fn[es] )
									din = varval;
								else
									din = "";
								imgdscs_want_basename = true;
							}
							else
							if( varname == "\\" )
							{
								std::string varval = fn.substr(es);
								if( fn[es] )
									din = varval;
								else
									din = "";
								imgdscs_want_basename = false;
							}
							else
#endif /* FIM_WANT_PIC_CCMT */
							if( fn[es] )
							{
								if ( fim_is_id(varname.c_str()) )
								{
									std::string varval = fn.substr(es);
									ns.setVariable(varname,Var(varval));
								}
								else
									; // not an id; TODO: may warn the user
							}
							else
							{
								std::string varname = fn.substr(vn,es-1-vn);
								ns.unsetVariable(varname);
							}
						}
					}
				}
			}
			else
#endif /* FIM_WANT_PIC_LVDN */
			if(std::getline(ls,fn,sc))
			{
				const bool aoec = true; // (propagate i:variables) also on empty commentary

				if( (std::getline(ls,ds,nl) /* non empty commentary */) || aoec)
				{
#if FIM_WANT_PIC_RCMT
					{
						size_t csi = ds.find("#!fim:",0);
						size_t csil = 6;

						if( csi != 0 )
							ld = ds; // cache new description
						else
						{
							// use last (cached) description
							char sc = ds[csil];

							switch(sc)
							{
								case('='): // #!fim:=
									ds = ld;
								break;
								case('+'): // #!fim:+
									ds = ld + ds.substr(csil);
								break;
								case('^'): // #!fim:^
									ds = ds.substr(csil) + ld;
								break;
								case('s'): // #!fim:s/from/to '/' not allowed in from or to
								{
									fim::string es = ds.substr(csil);
									size_t m = ((es).re_match("s/[^/]+/[^/]+"));
									if(m)
									{
										size_t n = es.find("/",2);
										std::string fs = es.substr(2,n-2), ts = es.substr(n+1);
										fim::string fds = ld;
										fds.substitute(fs.c_str(),ts.c_str());
										ds = fds.c_str();
									}
								}
								break;
							}
						}
					}
#endif /* FIM_WANT_PIC_RCMT */
					const bool would_remove = false;
					VNamespace cns;
					cns=ns;
					cns.cleanup();
					if (!would_remove)
					{
#if FIM_WANT_PIC_CCMT
						ds = cps + ds + cas;
#endif /* FIM_WANT_PIC_CCMT */
						ds = expand(ns,ds);
						if(! imgdscs_want_basename )
						{
							(*this)[din+fn]=ds;
#if FIM_WANT_PIC_LVDN
							vd_[std::string(din+fn)]=cns;
#endif /* FIM_WANT_PIC_LVDN */
						}
						else
						{
							(*this)[din+fim_basename_of(fn)]=ds;
#if FIM_WANT_PIC_LVDN
							vd_[din+fim_basename_of(fn)]=cns;
#endif /* FIM_WANT_PIC_LVDN */
						}
					}
					else
					{
						// TODO: FIXME: this branch waits to be activated.
						// ... remove pic
						const bool imgdscs_want_basename = true;
						ds = expand(ns,ds);
						if(! imgdscs_want_basename )
						{
							(*this)[din+fn]=ds;
#if FIM_WANT_PIC_LVDN
							vd_[std::string(din+fn)]=cns;
#endif /* FIM_WANT_PIC_LVDN */
						}
						else
						{
							(*this)[din+fim_basename_of(fn)]=ds;
#if FIM_WANT_PIC_LVDN
							vd_[din+fim_basename_of(fn)]=cns;
#endif /* FIM_WANT_PIC_LVDN */
						}
					}
				}
			}
		}
		reset();
#if FIM_WANT_PIC_LVDN
		// print(std::cout) << "\n";
		shrink_to_fit();
		// print(std::cout) << "\n";
#endif /* FIM_WANT_PIC_LVDN */
		// dt += getmilliseconds();
		// std::cout << fim::string("fetched images descriptions in ") << dt << " ms" << std::endl;
	}
	key_type fk(const mapped_type & sk) 
	{
		mapped_type v;
		if ( fo(sk,li_) != end() )
		{
			v = fi(sk)->first;
		}
		else
		{
			reset();
			if ( fo(sk,li_) != end() )
				v = fi(sk)->first;
		}
		return v;
	}
	void shrink_to_fit(void)
	{
		/* note we cannot it->first.shrink_to_fit(), */
#if FIM_USE_CXX11
		for( auto it =     begin();it !=     end(); ++it )
			it->second.shrink_to_fit();
#if FIM_WANT_PIC_LVDN
		for( auto it = vd_.begin();it != vd_.end(); ++it )
			it->second.shrink_to_fit();
#endif /* FIM_WANT_PIC_LVDN */
#endif /* FIM_USE_CXX11 */
	}
	size_t byte_size(void)const
	{
		size_t bs = size() + sizeof(*this);
		for( const_iterator it = begin();it != end(); ++it )
			bs += it->first.capacity() + sizeof(it->first),
			bs += it->second.capacity() + sizeof(it->second);
#if FIM_WANT_PIC_LVDN
		for( vd_t::const_iterator it = vd_.begin();it != vd_.end(); ++it )
			bs += it->first.capacity() + sizeof(it->first),
			bs += it->second.byte_size();
#endif /* FIM_WANT_PIC_LVDN */
		return bs;
	}
	std::ostream& print(std::ostream& os)const
	{
		os << (size()) << " entries in " << byte_size() << " bytes";
		return os;
	}
#if FIM_WANT_PIC_LVDN
	string get_variables_id_list(size_t up_to_max)const
	{
		// FIXME: not using up_to_max properly yet (by popularity, etc) !
		size_t cnt = 0;
		std::ostringstream vls;
		fim_var_id_set vis;
		for( vd_t::const_iterator li = vd_.begin();li != vd_.end(); ++li )
			li->second.get_id_list(vis);

		if(vis.size())
			vls << "there are " << vis.size() << " variable ids:";
		else
			vls << "no variable ids.";

		for( fim_var_id_set::const_iterator li = vis.begin();li != vis.end()
			&& cnt < up_to_max
			; ++li )
			cnt++,
			vls << " " << *li;
		if( vis.size() > up_to_max ) 
			vls << " ...";
		return vls.str();
	}
#endif /* FIM_WANT_PIC_LVDN */

#if FIM_WANT_PIC_LVDN
	string get_values_list_for(const string& id, size_t up_to_max)const
	{
		// FIXME: not using up_to_max properly yet (by popularity, etc) !
		string vls;
		size_t cnt = 0;
		//fim_var_val_set vvs; // FIXME: Var is not yet ready to be used in a std::set
		fim_var_id_set vvs;//FIXME: temporary
		for( vd_t::const_iterator li = vd_.begin();li != vd_.end(); ++li )
			if( li->second.isSetVar(id) )
				vvs.insert(li->second.getStringVariable(id));
		if(vvs.size())
			vls += string("there are "),
			vls += string(Var(static_cast<fim_int>(vvs.size()))),
			vls += string(" values for variable "),
			vls += id,
			vls += string(":");
		else
			vls += string("no instances for variable "),
			vls += id,
			vls += string(".");

		//for( fim_var_val_set::const_iterator li = vvs.begin();li != vvs.end()
		for( fim_var_id_set ::const_iterator li = vvs.begin();li != vvs.end()
			&& cnt < up_to_max
			; ++li )
			cnt++,
			vls += " '",
			vls += *li, // TODO: shall unescape this value 
			vls += "'";
			//vls += li->getString(); // TODO: shall unescape this value 
		if( vvs.size() > up_to_max ) 
			vls += " ...";
		return vls;
	}
#endif /* FIM_WANT_PIC_LVDN */
	/*
	std::ostream& print_descs(std::ostream& os, fim_char_t sc)const
	{
		for( const_iterator it = begin();it != end(); ++it )
			os << it->first << sc << it->second << "\n";
		return os;
	}
	*/
}; /* ImgDscs */
	std::ostream& operator<<(std::ostream& os, const ImgDscs & id);
#endif /* FIM_WANT_PIC_CMTS */
#endif /* FIM_IMAGE_H */
