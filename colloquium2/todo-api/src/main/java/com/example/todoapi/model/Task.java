package com.example.todoapi.model;

public class Task {
    private int id;
    private String title;
    private String description;
    private TaskStatus status;

    public Task() {
        this.status = TaskStatus.TODO;
    }

    public Task(int id, String title, String description, TaskStatus status) {
        this.id = id;
        this.title = title;
        this.description = description;
        this.status = status;
    }

    public Task(String title, String description, TaskStatus status) {
        this.title = title;
        this.description = description;
        this.status = status;
    }

    public int getId() { return id; }
    public String getTitle() { return title; }
    public String getDescription() { return description; }
    public TaskStatus getStatus() { return status; } // Возвращает enum

    public void setId(int id) { this.id = id; }
    public void setTitle(String title) { this.title = title; }
    public void setDescription(String description) { this.description = description; }
    public void setStatus(TaskStatus status) { this.status = status; }

    public void setStatus(String status) {
        this.status = TaskStatus.fromString(status);
    }

    public String getStatusValue() {
        return status.getValue();
    }

    @Override
    public String toString() {
        return "Task{" +
                "id=" + id +
                ", title='" + title + '\'' +
                ", description='" + description + '\'' +
                ", status=" + status.getValue() +
                '}';
    }
}