from PyPDF2 import PdfReader, PdfWriter

# File paths
pdf1_path = 'NHF_DOC.pdf'
pdf2_path = 'latex.pdf'
output_path = 'CQREYD_NHF.pdf'

# Create a PDF writer object
writer = PdfWriter()

# Process the first PDF (remove the last page)
reader1 = PdfReader(pdf1_path)
number_of_pages_pdf1 = len(reader1.pages)
for i in range(number_of_pages_pdf1 - 1):  # Exclude the last page
    writer.add_page(reader1.pages[i])

# Process the second PDF (append all pages)
reader2 = PdfReader(pdf2_path)
for page in reader2.pages:
    writer.add_page(page)

# Write the result to a new file
with open(output_path, "wb") as f:
    writer.write(f)

