import requests
import json
import re
from requests.packages.urllib3.exceptions import InsecureRequestWarning

# Ignore SSL warnings
requests.packages.urllib3.disable_warnings(InsecureRequestWarning)

# Authentication information for the source and destination GitLab instances
source_gitlab_url = "https://source-gitlab.com"
source_private_token = "YOUR_SOURCE_PRIVATE_TOKEN"

destination_gitlab_url = "https://destination-gitlab.com"
destination_private_token = "YOUR_DESTINATION_PRIVATE_TOKEN"

# Function to create an issue in the destination GitLab instance
def create_issue(project_id, issue_data):
    headers = {"PRIVATE-TOKEN": destination_private_token}
    response = requests.post(f"{destination_gitlab_url}/api/v4/projects/{project_id}/issues", headers=headers, json=issue_data, verify=False)
    created_issue = response.json()
    return created_issue

# Function to create a note for an issue in the destination GitLab instance
def create_note(project_id, issue_iid, note_data):
    headers = {"PRIVATE-TOKEN": destination_private_token}
    response = requests.post(f"{destination_gitlab_url}/api/v4/projects/{project_id}/issues/{issue_iid}/notes", headers=headers, json=note_data, verify=False)
    created_note = response.json()
    return created_note

# Function to download an image from the source GitLab instance
def download_image(image_url):
    response = requests.get(image_url, verify=False)
    return response.content

# Function to upload an image to the destination GitLab instance
def upload_image(image_data):
    headers = {"PRIVATE-TOKEN": destination_private_token}
    response = requests.post(f"{destination_gitlab_url}/api/v4/projects/{project_id}/uploads", headers=headers, files={"file": image_data}, verify=False)
    uploaded_image = response.json()
    return uploaded_image["url"]

# Read issue data from the JSON file
with open("issues_data.json", "r") as json_file:
    all_issues = json.load(json_file)

# Loop through the issues
for issue_data in all_issues:
    # Change the state of the issue to closed if it was closed in the source GitLab
    if issue_data["state"] == "closed":
        issue_data["state"] = "closed"
    
    # Download and upload images referenced in the issue description
    description = issue_data.get("description", "")
    image_urls = re.findall(r"!\[.*?\]\((.*?)\)", description)
    for image_url in image_urls:
        image_data = download_image(image_url)
        uploaded_image_url = upload_image(image_data)
        description = description.replace(image_url, uploaded_image_url)
    issue_data["description"] = description
    
    # Create the issue in the destination GitLab
    created_issue = create_issue(project_id, issue_data)
    
    # Loop through the notes
    for note_data in issue_data.get("notes", []):
        # Create the note for the issue in the destination GitLab
        create_note(project_id, created_issue["iid"], note_data)

print("Issue data has been posted to the new GitLab instance successfully.")
