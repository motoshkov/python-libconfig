Summary: A python interface to libconfig
Name: python-libconfig
Version: 0.0.2
Release: 3
License: bsd
Group: Development/Libraries
Source0: %{name}-%{version}.tar.gz
URL: https://github.com/cnangel/python-libconfig/

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
BuildRequires: boost-devel python-devel libconfig libconfig-devel python-setuptools
Requires: libconfig

%description
Python interface to libconfig.

%prep
%setup -q

%build
export libdirname=%{_lib}
CFLAGS="$RPM_OPT_FLAGS" python setup.py build

%install
rm -rf $RPM_BUILD_ROOT
export libdirname=%{_lib}
python setup.py install --root=$RPM_BUILD_ROOT --record=INSTALLED_FILES

%clean
rm -rf $RPM_BUILD_ROOT

%files -f INSTALLED_FILES
%defattr(-,root,root)
%{_libdir}/python?.?/site-packages/*.so

%changelog

