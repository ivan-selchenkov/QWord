Name: qword
Version: 2.0
Release: 2
Summary: Application for learning foreign words
Group: Education/Languages
License: GPL
Source0: qword-2.0.tar.gz
Source1: qword.desktop
BuildRoot: %{_tmppath}/%{name}-%{version}-build 

#########################################################################################  
# SuSE, openSUSE  
#########################################################################################  
%if 0%{?suse_version}  
BuildRequires:  update-desktop-files  
BuildRequires:  libqt4-devel  
%endif  
 #########################################################################################  
# Fedora, RHEL or CentOS  
#########################################################################################  
%if 0%{?fedora_version} || 0%{?rhel_version} || 0%{?centos_version}  
BuildRequires:  qt4-devel  
BuildRequires:  desktop-file-utils  
%endif  

%description
Application for learning foreign words

%prep
%setup -q -n %{name}-%{version}

%build
qmake-qt4
make %{?jobs:-j%jobs}

%install
mkdir -p $RPM_BUILD_ROOT/%{_datadir}/pixmaps
mkdir -p $RPM_BUILD_ROOT/%{_bindir}  
install QWord $RPM_BUILD_ROOT/%{_bindir}
install -m 644 images/qword.svg $RPM_BUILD_ROOT/%{_datadir}/pixmaps

%if 0%{?suse_version}
 %suse_update_desktop_file -i %{name} Education
%elseif 0%{?fedora_version}
  desktop-file-install --dir=%{buildroot}%{_datadir}/applications/ --add-category Education --add-category Languages applications/qword.desktop
%endif

%files
%defattr(-,root,root)
%{_bindir}/QWord
%{_datadir}/pixmaps/qword.svg
%{_datadir}/applications/qword.desktop

%clean
rm -rf $RPM_BUILD_ROOT

%changelog
* Fri May 15 2009 Ivan Selchenkov
- Tray mode added
- Some interface and error correction
* Tue May 12 2009 Ivan Selchenkov
- Initial build


