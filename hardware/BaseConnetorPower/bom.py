import untangle
import re
import xlsxwriter

target = 'bittag-charger.xlsx'
source = 'bittag-charger.xml'

workbook = xlsxwriter.Workbook(target)
worksheet = workbook.add_worksheet()
row = 0

obj = untangle.parse(source)
components = obj.export.components
try:
    data = ('Designator','Value','Package','Populate','MPN')
    worksheet.write_row('A1',data)
except:
    print( "couldn't write worksheet")

for comp in components.comp:
    row = row + 1
    Designator =  comp['ref']
    Value      =  comp.value.cdata
    footprint = comp.footprint.cdata
    footprint = re.sub(r".*:","",footprint)
    fields = {}
    try:
        for f in comp.fields.field:
            fields[f['name']] = f.cdata
    except:
        pass
    Populate = "1"
    if 'Populate' in fields:
        Populate = fields['Populate']
    MPN = ""
    if 'Mouser' in fields:
        MPN = fields['Mouser']
    if 'Digikey' in fields:
        MPN = fields['Digikey']
    if 'MFP' in fields:
        MPN = fields['MFP']
    if 'MPN' in fields:
        MPN = fields['MPN']
    if 'Macrofab' in fields:
        MPN = fields['Macrofab']
    worksheet.write(row,0,Designator)
    worksheet.write(row,1,Value)
    worksheet.write(row,2,footprint)
    worksheet.write(row,3,int(Populate))
    worksheet.write(row,4,MPN)

workbook.close()
    


