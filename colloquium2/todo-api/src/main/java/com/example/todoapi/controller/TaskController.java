package com.example.todoapi.controller;

import com.example.todoapi.model.Task;
import com.example.todoapi.repository.TaskRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/tasks")
public class TaskController {

    private final TaskRepository taskRepository;

    @Autowired
    public TaskController(TaskRepository taskRepository) {
        this.taskRepository = taskRepository;
    }

    @GetMapping
    public ResponseEntity<List<Task>> getAllTasks() {
        List<Task> tasks = taskRepository.findAll();
        return ResponseEntity.ok(tasks);
    }

    @GetMapping("/{id}")
    public ResponseEntity<Task> getTaskById(@PathVariable int id) {
        Task task = taskRepository.findById(id);

        if (task == null) {
            return ResponseEntity.notFound().build();
        }

        return ResponseEntity.ok(task);
    }

    @PostMapping
    public ResponseEntity<Task> createTask(@RequestBody Task newTask) {
        if (newTask.getTitle() == null || newTask.getTitle().trim().isEmpty()) {
            return ResponseEntity.badRequest().build();
        }

        Task savedTask = taskRepository.save(newTask);

        return ResponseEntity.status(HttpStatus.CREATED).body(savedTask);
    }

    @PutMapping("/{id}")
    public ResponseEntity<Task> updateTask(@PathVariable int id, @RequestBody Task updatedTask) {
        if (updatedTask.getTitle() == null || updatedTask.getTitle().trim().isEmpty() ||
                updatedTask.getStatus() == null) {
            return ResponseEntity.badRequest().build();
        }

        Task task = taskRepository.update(id, updatedTask);

        if (task == null) {
            return ResponseEntity.notFound().build();
        }

        return ResponseEntity.ok(task);
    }

    @PatchMapping("/{id}")
    public ResponseEntity<Task> partialUpdateTask(@PathVariable int id, @RequestBody Task taskUpdates) {
        Task existingTask = taskRepository.findById(id);

        if (existingTask == null) {
            return ResponseEntity.notFound().build();
        }

        Task updatedTask = new Task(
                existingTask.getId(),
                existingTask.getTitle(),
                existingTask.getDescription(),
                existingTask.getStatus()
        );

        if (taskUpdates.getTitle() != null && !taskUpdates.getTitle().trim().isEmpty()) {
            updatedTask.setTitle(taskUpdates.getTitle());
        }

        if (taskUpdates.getDescription() != null) {
            updatedTask.setDescription(taskUpdates.getDescription());
        }

        if (taskUpdates.getStatus() != null) {
            updatedTask.setStatus(taskUpdates.getStatus());
        }

        Task savedTask = taskRepository.update(id, updatedTask);
        return ResponseEntity.ok(savedTask);
    }

    @DeleteMapping("/{id}")
    public ResponseEntity<Void> deleteTask(@PathVariable int id) {
        boolean deleted = taskRepository.delete(id);

        if (!deleted) {
            return ResponseEntity.notFound().build();
        }

        return ResponseEntity.noContent().build();
    }
}