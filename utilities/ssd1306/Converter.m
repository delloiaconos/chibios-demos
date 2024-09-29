%% 
 % Image Conversion utility for SSD1306 library.
 % Copyright (C) 2024 
 % - Salvatore Dello Iacono [delloiaconos@gmail.com]
 % - Domenico Rega [dodorega@gmail.com]
 %
 % This is free software; you can redistribute it and/or modify
 % it under the terms of the GNU General Public License as published by
 % the Free Software Foundation; either version 3 of the License, or
 % (at your option) any later version.
 %
 % This is distributed in the hope that it will be useful,
 % but WITHOUT ANY WARRANTY; without even the implied warranty of
 % MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 % GNU General Public License for more details.
 %
 %  You should have received a copy of the GNU General Public License
 %  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 %

function Image2Display( filename )
    
    [~,fName,~] = fileparts(filename);

    img = imread( filename );
    img = imresize(img, [64 128]);
    img = imbinarize(img);

    newName = string( join( regexp( fName ,'[a-zA-Z0-9_]*', 'Match'), '' ) );
    
    re = regexp( newName ,'[a-zA-Z][a-zA-Z0-9_]*', 'Match' );
    if isempty( re )
        newName = strcat( 'tmp', newName );
    end
    newName = join( regexp( newName ,'[a-zA-Z][a-zA-Z0-9_]*', 'Match'), '' );

    fid=fopen( strcat( newName, '.h'), 'wt');

    fprintf(fid, "#ifndef __%s_H__\n", upper(newName) );
    fprintf(fid, "#define __%s_H__\n\n", upper(newName) );
    
    fprintf(fid, "/* This file has been automatically generated from '%s'. */\n\n", filename );
    fprintf(fid, "static const ssd1306_color_t %s_data [] = { \n", lower(newName) );
    
    for x=1:64
        fprintf(fid, "\t" );
        for y=1:128
            fprintf(fid, "%d, ", img(x,y));
        end
        fprintf(fid, "\n" );
    end
    fprintf(fid, "\n\t};\n");    
    fprintf(fid, "#endif // __%s_H__\n", upper(newName) );

    fclose(fid);

end


%% Usage Example

[file,location] = uigetfile({'*.bmp;*.png;*.jpg','Images File'; ...
                             '*.*', 'All Files (*.*)'}, ...
                          'Image Selector' );
if isequal(file,0)
   disp('User selected Cancel');
else
   Image2Display( fullfile( location, file ) );
end
