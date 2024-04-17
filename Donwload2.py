import os
import requests
import json
import re

# Ignore SSL warnings
requests.packages.urllib3.disable_warnings(requests.packages.urllib3.exceptions.InsecureRequestWarning)

# Authentication information for the GitLab instance
gitlab_url = "https://gitlab.com"
private_token = "YOUR_PRIVATE_TOKEN"
project_id = "YOUR_PROJECT_ID"

# Directory to save downloaded files and images
download_directory = "downloads"

# Function to download a file from the GitLab instance
def download_file(file_url, local_filename):
    headers = {"PRIVATE-TOKEN": private_token}
    response = requests.get(f"{gitlab_url}/{file_url}", headers=headers, verify=False)
    with open(local_filename, 'wb') as file:
        file.write(response.content)

# Create the download directory if it doesn't exist
os.makedirs(download_directory, exist_ok=True)

# Get issues from the GitLab project
def get_issues(project_id):
    headers = {"PRIVATE-TOKEN": private_token}
    response = requests.get(f"{gitlab_url}/api/v4/projects/{project_id}/issues", headers=headers, verify=False)
    issues = response.json()
    return issues

# Download files and images from the issue descriptions
def download_attachments_from_issues(issues):
    for issue in issues:
        description = issue['description']
        if description:
            # Find all image and file URLs in the description
            urls = re.findall(r'!\[.*?\]\((.*?)\)', description)
            for url in urls:
                download_file(url, os.path.join(download_directory, os.path.basename(url)))

# Get issues from the GitLab project
issues = get_issues(project_id)

# Download attachments from the issues' descriptions
download_attachments_from_issues(issues)

print("Download completed. Files and images have been saved in the download directory.")
