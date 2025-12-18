package com.example.todoapi.model;

public enum TaskStatus{
    TODO("todo"),
    IN_PROGRESS("in_progress"),
    DONE("done");

    private final String value;

    TaskStatus(String value){
        this.value = value;
    }

    public String getValue(){
        return value;
    }

    public static TaskStatus fromString(String text) {
        if (text == null || text.trim().isEmpty()) {
            return TODO;
        }

        for (TaskStatus status : TaskStatus.values()) {
            if (status.value.equalsIgnoreCase(text.trim())) {
                return status;
            }
        }

        return TODO;
    }
}
