import requests
import json
import os

source_tower_url = os.getenv('SOURCE_TOWER_URL')
source_token = os.getenv('SOURCE_TOKEN')
destination_tower_url = os.getenv('DESTINATION_TOWER_URL')
destination_token = os.getenv('DESTINATION_TOKEN')

headers_source = {"Authorization": f"Bearer {source_token}"}
headers_destination = {"Authorization": f"Bearer {destination_token}", "Content-Type": "application/json"}

def get_data(url, headers):
    response = requests.get(url, headers=headers)
    response.raise_for_status()
    return response.json()

def post_data(url, headers, data):
    response = requests.post(url, headers=headers, json=data)
    response.raise_for_status()
    return response.json()

def extract_and_map_data(endpoint):
    data_source = get_data(f"{source_tower_url}/api/v2/{endpoint}", headers_source)
    mapped_data = []
    id_map = {}
    for item in data_source['results']:
        new_item = post_data(f"{destination_tower_url}/api/v2/{endpoint}", headers_destination, item)
        mapped_data.append(new_item)
        id_map[item['id']] = new_item['id']
    return id_map, mapped_data

# Extract and map organizations
organization_id_map, organizations = extract_and_map_data("organizations")

# Extract and map credentials
credential_id_map, credentials = extract_and_map_data("credentials")

# Save the mappings
with open("organization_id_map.json", "w") as f:
    json.dump(organization_id_map, f, indent=4)

with open("credential_id_map.json", "w") as f:
    json.dump(credential_id_map, f, indent=4)

with open("organizations.json", "w") as f:
    json.dump(organizations, f, indent=4)

with open("credentials.json", "w") as f:
    json.dump(credentials, f, indent=4)
&&&&&&&&&&&&
import requests
import json
import os

destination_tower_url = os.getenv('DESTINATION_TOWER_URL')
destination_token = os.getenv('DESTINATION_TOKEN')
headers_destination = {"Authorization": f"Bearer {destination_token}", "Content-Type": "application/json"}

def post_data(url, headers, data):
    response = requests.post(url, headers=headers, json=data)
    response.raise_for_status()
    return response.json()

# Load the mappings
with open("organization_id_map.json") as f:
    organization_id_map = json.load(f)

with open("credential_id_map.json") as f:
    credential_id_map = json.load(f)

# Load the projects data
with open("projects.json") as f:
    projects = json.load(f)

# Post the projects to the destination
for project in projects:
    project['organization'] = organization_id_map[project['organization']]
    if 'credential' in project and project['credential'] in credential_id_map:
        project['credential'] = credential_id_map[project['credential']]
    post_data(f"{destination_tower_url}/api/v2/projects", headers_destination, project)

print("Projects have been posted successfully.")
