import os
import requests
import json
import re

# Ignore SSL warnings
requests.packages.urllib3.disable_warnings(requests.packages.urllib3.exceptions.InsecureRequestWarning)

# Authentication information for the GitLab instance
gitlab_url = "https://gitlab.com"
private_token = "YOUR_PRIVATE_TOKEN"

# Directory to save downloaded files
download_directory = "downloads"

# Function to download a file from a URL
def download_file(url, local_filename):
    response = requests.get(url, verify=False)
    with open(local_filename, 'wb') as file:
        file.write(response.content)

# Create the download directory if it doesn't exist
os.makedirs(download_directory, exist_ok=True)

# Read the issue data from a JSON file
with open("issue_data.json", "r") as json_file:
    issue_data = json.load(json_file)

# Extract and download files from the URLs in the issue data
for issue in issue_data:
    description = issue.get("description", "")
    # Find all URLs in the description
    urls = re.findall(r'\((.*?)\)', description)
    for url in urls:
        # Check if the URL is an upload URL
        if url.startswith("/uploads"):
            full_url = f"{gitlab_url}{url}"
            filename = os.path.basename(url)
            download_file(full_url, os.path.join(download_directory, filename))

print("Download completed. Files have been saved in the downloads directory.")
