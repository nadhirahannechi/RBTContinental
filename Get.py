import requests
import json
import os

source_tower_url = os.getenv('SOURCE_TOWER_URL')
source_token = os.getenv('SOURCE_TOKEN')
organization_id = int(os.getenv('ORGANIZATION_ID'))
headers = {"Authorization": f"Bearer {source_token}"}

def get_data(endpoint):
    response = requests.get(f"{source_tower_url}/{endpoint}", headers=headers)
    response.raise_for_status()
    return response.json()

def filter_data_by_organization(data, org_id):
    return [item for item in data['results'] if item['organization'] == org_id]

# Retrieve all data
organizations = get_data("organizations")
users = get_data("users")
job_templates = get_data("job_templates")
projects = get_data("projects")
credential_types = get_data("credential_types")

# Filter data by the given organization ID
filtered_organizations = [org for org in organizations['results'] if org['id'] == organization_id]
filtered_users = filter_data_by_organization(users, organization_id)
filtered_job_templates = filter_data_by_organization(job_templates, organization_id)
filtered_projects = filter_data_by_organization(projects, organization_id)

# Save the filtered data to JSON files
with open("organizations.json", "w") as f:
    json.dump(filtered_organizations, f, indent=4)
with open("users.json", "w") as f:
    json.dump(filtered_users, f, indent=4)
with open("job_templates.json", "w") as f:
    json.dump(filtered_job_templates, f, indent=4)
with open("projects.json", "w") as f:
    json.dump(filtered_projects, f, indent=4)
with open("credential_types.json", "w") as f:
    json.dump(credential_types['results'], f, indent=4)  # Assuming credential types are not organization-specific
