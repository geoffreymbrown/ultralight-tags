import csv
import sys
import os

root = os.path.basename(os.getcwd())
print(root)

bom = dict()
infilename = root
with open(infilename) as infile:
    for line in infile:
        print(line.strip())

with open(infilename,newline='') as csvfile:
    hdr = csvfile.readline().split(',')
    reader = csv.DictReader(csvfile,fieldnames=hdr)
    for row in reader:
        print(row)
        if 'Reference' in row:
            ref = row['Reference']
            del row['Reference']
            print('adding',ref)
            bom[ref] = row
        else: 
            print('no referene in', row)

writer = None
with open(root+'-pos-bom.csv', 'w', newline='') as outfile:
    with open(root+'-all-pos.csv', newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            if 'Ref' in row and row['Ref'] in bom:
                row.update(bom[row['Ref']].items())
                if not writer:
                    fieldnames = row.keys()
                    writer = csv.DictWriter(outfile, fieldnames=fieldnames)
                    writer.writeheader()
                writer.writerow(row)
            elif 'Ref' in row:
                print("couldn't find ", row['Ref'])
            else:
                print("no 'Ref' in row", row)

print(bom.keys())